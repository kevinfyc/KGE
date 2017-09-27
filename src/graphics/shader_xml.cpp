//
//  shader_xml.cpp
//  libkge
//
//  Created by kevin on 17-9-26.
//
//

#include "shader_xml.h"
#include "io\section_factory.h"
#include "render_queue.h"

namespace kge
{

	const char* VERTEX_ATTR_TYPES[(int)VertexAttributeType::Count] =
	{
		"Vertex",
		"Color",
		"Texcoord",
		"Texcoord2",
		"Normal",
		"Tangent",
		"BlendWeight",
		"BlendIndices"
	};

	const int VERTEX_ATTR_SIZES[(int)VertexAttributeType::Count] = {
		12, 16, 8, 8, 12, 16, 16, 16
	};

	const int VERTEX_ATTR_OFFSETS[(int)VertexAttributeType::Count] =
	{
		0, 12, 28, 36, 44, 56, 72, 88
	};

	const uint32 VERTEX_STRIDE = 104;

	void ShaderXML::Clear()
	{
		passes.clear();
		vss.clear();
		pss.clear();
		rss.clear();
	}

	void ShaderXML::Load(const std::string& path)
	{
		Clear();

		Ref<ISection> section = SectionFactory::Load(path, SectionType::Xml);
		if (section)
		{
			Ref<ISection> shader = section->Read("Shader");
			name = shader->GetFirstAttribute("name");
			std::string shader_queue = shader->GetFirstAttribute("queue");

			if (shader_queue == "Background")
				queue = (uint32)RenderQueue::Background;
			else if (shader_queue == "Geometry")
				queue = (uint32)RenderQueue::Geometry;
			else if (shader_queue == "AlphaTest")
				queue = (uint32)RenderQueue::AlphaTest;
			else if (shader_queue == "Transparent")
				queue = (uint32)RenderQueue::Transparent;
			else if (shader_queue == "Overlay")
				queue = (uint32)RenderQueue::Overlay;

			SectionPtrVector children;
			shader->GetChildren(children);
			for (Ref<ISection> node : children)
			{
				std::string type = node->CTag();
				std::string node_name = node->GetFirstAttribute("name");

				if (type == "Pass")
				{
					Pass pass;
					pass.name = name;
					pass.vs = node->GetFirstAttribute("vs");
					pass.ps = node->GetFirstAttribute("ps");
					pass.rs = node->GetFirstAttribute("rs");
					passes.push_back(pass);
				}
				else if (type == "VertexShader")
				{
					VertexShader vs;
					vs.name = name;
					vs.uniform_buffer.binding = -1;
					vs.uniform_buffer.size = 0;

					SectionPtrVector children_vs;
					node->GetChildren(children_vs);

					for (Ref<ISection> vs_node : children_vs)
					{
						std::string vs_type = vs_node->CTag();

						if (vs_type == "UniformBuffer")
						{
							vs.uniform_buffer.name = vs_node->GetFirstAttribute("name");
							vs.uniform_buffer.binding = atoi(vs_node->GetFirstAttribute("binding").c_str());

							uint32 offset = 0;

							SectionPtrVector children_ub;
							vs_node->GetChildren(children_ub);

							for (Ref<ISection> ub_node : children_ub)
							{
								std::string ub_type = ub_node->CTag();

								if (ub_type == "Uniform")
								{
									Uniform uniform;
									uniform.name = ub_node->GetFirstAttribute("name");
									uniform.size = atoi(ub_node->GetFirstAttribute("size").c_str());

									uniform.offset = offset;
									offset += uniform.size;

									vs.uniform_buffer.uniforms.push_back(uniform);
								}
								ub_node = nullptr;
							}

							vs.uniform_buffer.size = offset;
						}
						else if (vs_type == "Include")
						{
							std::string include;
							include = vs_node->GetFirstAttribute("name");
							vs.includes.push_back(include);
						}
						else if (vs_type == "Source")
						{
							vs.src = vs_node->GetString();
						}
						else if (vs_type == "VertexAttribute")
						{
							VertexAttribute attr;
							attr.name = vs_node->GetFirstAttribute("name");
							attr.location = atoi(vs_node->GetFirstAttribute("location").c_str());

							for (int i = 0; i < (int)VertexAttributeType::Count; ++i)
							{
								if (attr.name == VERTEX_ATTR_TYPES[i])
								{
									attr.type = (VertexAttributeType)i;
									attr.size = VERTEX_ATTR_SIZES[i];
									attr.offset = VERTEX_ATTR_OFFSETS[i];
									break;
								}
							}

							vs.attrs.push_back(attr);
						}

						vs_node = nullptr;
					}

					vs.stride = VERTEX_STRIDE;
					vss.push_back(vs);
				}
				else if (type == "PixelShader")
				{
					PixelShader ps;
					ps.name = name;
					ps.uniform_buffer.binding = -1;
					ps.uniform_buffer.size = 0;

					SectionPtrVector children_ps;
					node->GetChildren(children_ps);
					for (Ref<ISection> ps_node : children_ps)
					{
						std::string ps_type = ps_node->CTag();

						if (ps_type == "UniformBuffer")
						{
							ps.uniform_buffer.name = ps_node->GetFirstAttribute("name");
							ps.uniform_buffer.binding = atoi(ps_node->GetFirstAttribute("binding").c_str());

							uint32 offset = 0;

							SectionPtrVector children_ub;
							ps_node->GetChildren(children_ub);
							for (Ref<ISection> ub_node : children_ub)
							{
								std::string ub_type = ub_node->CTag();

								if (ub_type == "Uniform")
								{
									Uniform uniform;
									uniform.name = ub_node->GetFirstAttribute("name");
									uniform.size = atoi(ub_node->GetFirstAttribute("size").c_str());

									uniform.offset = offset;
									offset += uniform.size;

									ps.uniform_buffer.uniforms.push_back(uniform);
								}

								ub_node = nullptr;
							}

							ps.uniform_buffer.size = offset;
							ps_node = nullptr;
						}
						else if (ps_type == "Sampler")
						{
							Sampler sampler;
							sampler.name = ps_node->GetFirstAttribute("name");
							sampler.binding = atoi(ps_node->GetFirstAttribute("binding").c_str());
							sampler.default_tex = ps_node->GetFirstAttribute("default");
						}
						else if (ps_type == "Include")
						{
							std::string include;
							include = ps_node->GetFirstAttribute("name");
							ps.includes.push_back(include);
						}
						else  if (ps_type == "Source")
						{
							ps.src = ps_node->GetString();
						}
					}

					pss.push_back(ps);
				}
				else if (type == "RenderState")
				{
					RenderState rs;
					rs.name = name;

					SectionPtrVector children_rs;
					node->GetChildren(children_rs);
					for (Ref<ISection> rs_node : children_rs)
					{
						std::string rs_type = rs_node->CTag();
						if (rs_type == "Cull")
						{
							rs.Cull = rs_node->GetFirstAttribute("value");
						}
						else if (rs_type == "ZTest")
						{
							rs.ZTest = rs_node->GetFirstAttribute("value");
						}
						else if (rs_type == "ZWrite")
						{
							rs.ZWrite = rs_node->GetFirstAttribute("value");
						}
						else if (rs_type == "AlphaTest")
						{
							rs.AlphaTest = rs_node->GetFirstAttribute("value");
						}
						else if (rs_type == "ColorMask")
						{
							rs.ColorMask = rs_node->GetFirstAttribute("value");
						}
						else if (rs_type == "Blend")
						{
							rs.Blend.enable = true;
							std::string enable;
							enable = rs_node->GetFirstAttribute("enable");
							if (enable == "Off")
								rs.Blend.enable = false;

							if (rs.Blend.enable)
							{
								rs.Blend.src = rs_node->GetFirstAttribute("src");
								rs.Blend.dst = rs_node->GetFirstAttribute("dst");
								rs.Blend.src_a = rs_node->GetFirstAttribute("src_a");
								rs.Blend.dst_a = rs_node->GetFirstAttribute("dst_a");

								if (rs.Blend.src_a.empty())
									rs.Blend.src_a = rs.Blend.src;
								if (rs.Blend.dst_a.empty())
									rs.Blend.dst_a = rs.Blend.dst;
							}
						}
						else if (rs_type == "Offset")
						{
							rs.offset.enable = true;
							rs.offset.factor = atof(rs_node->GetFirstAttribute("factor").c_str());
							rs.offset.units = atof(rs_node->GetFirstAttribute("units").c_str());
						}
						else if (rs_type == "Stencil")
						{
							rs.stencil.enable = true;
							rs.stencil.RefValue = atoi(rs_node->GetFirstAttribute("RefValue").c_str());
							rs.stencil.ReadMask = atoi(rs_node->GetFirstAttribute("ReadMask").c_str());
							rs.stencil.WriteMask = atoi(rs_node->GetFirstAttribute("WriteMask").c_str());

							rs.stencil.Comp = rs_node->GetFirstAttribute("Comp");
							rs.stencil.Pass = rs_node->GetFirstAttribute("Pass");
							rs.stencil.Fail = rs_node->GetFirstAttribute("Fail");
							rs.stencil.ZFail = rs_node->GetFirstAttribute("ZFail");
						}

						rs_node = nullptr;
					}

					rss.push_back(rs);
				}

				node = nullptr;
			}
		}
	}
}

