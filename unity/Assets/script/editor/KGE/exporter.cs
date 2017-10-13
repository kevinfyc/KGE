using UnityEditor;
using UnityEngine;
using System.IO;

public class export : exporter_mesh {

    const int BONE_MAX = 80;

    [MenuItem("KGE/Export/GameObject")]
    public static void ExportGameObjectAll()
    {
        ExportGameObject(false);
    }

    [MenuItem("KGE/Export/GameObject (Active Only)")]
    public static void ExportGameObjectActiveOnly()
    {
        ExportGameObject(true);
    }

    static void ExportGameObject(bool active_only)
    {
        var obj = Selection.activeGameObject;
        if (obj == null)
        {
            return;
        }

        MemoryStream ms = Init();

        WriteTransform(obj.transform, active_only);

        var file_path = new FileInfo(m_out_path + "/Assets/" + obj.name + ".prefab");
        if (!file_path.Directory.Exists)
        {
            Directory.CreateDirectory(file_path.Directory.FullName);
        }
        File.WriteAllBytes(file_path.FullName, ms.ToArray());

        Deinit();

        Log("export prefab " + obj.name + " done.");
    }

    static void WriteTransform(Transform t, bool active_only)
    {
        WriteString(t.name);
        m_writer.Write(t.gameObject.activeSelf);
        bool is_static = (GameObjectUtility.GetStaticEditorFlags(t.gameObject) & StaticEditorFlags.BatchingStatic) != 0;
        m_writer.Write(is_static);

        WriteVector3(t.localPosition);
        WriteQuaternion(t.localRotation);
        WriteVector3(t.localScale);
        m_writer.Write(t.GetInstanceID());

        int com_count = 0;
        var renderer = t.GetComponent<Renderer>();
        var anim = t.GetComponent<Animation>();
        var particle_system = t.GetComponent<ParticleSystem>();

        bool split_skin = false;


        if (renderer != null && split_skin == false)
        {
            com_count++;
        }
        if (anim != null)
        {
            com_count++;
        }
        if (particle_system != null)
        {
            com_count++;
        }

        m_writer.Write(com_count);

        if (renderer != null && split_skin == false)
        {
            string type_name = "";

            if (renderer is MeshRenderer)
            {
                type_name = "MeshRenderer";
            }

            WriteString(type_name);

            switch (type_name)
            {
                case "MeshRenderer":
                    WriteMeshRenderer(renderer as MeshRenderer);
                    break;
            }
        }

        int child_count = t.childCount;

        if (active_only)
        {
            int active_count = 0;
            for (int i = 0; i < child_count; i++)
            {
                var child = t.GetChild(i);
                if (child.gameObject.activeInHierarchy)
                {
                    active_count++;
                }
            }

            m_writer.Write(active_count);
        }
        else
        {
            m_writer.Write(child_count);
        }

        for (int i = 0; i < child_count; i++)
        {
            var child = t.GetChild(i);

            if (child.gameObject.activeInHierarchy || !active_only)
            {
                WriteTransform(child, active_only);
            }
        }
    }
}