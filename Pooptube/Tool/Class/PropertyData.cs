using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Tool.Class
{
    /// <summary>
    /// Sub Category Interface Class
    /// </summary>
    [TypeConverter(typeof(ExpandableObjectConverter))]
    public class SubCategory
    {
        public override string ToString() { return String.Empty; }
    }

    public class Settings
    {
        [Browsable(false)]
        public Core.Node target { get; private set; }

        public void InitValue(Core.Node t)
        {
            target = t;
            Type = target.ToString();
           
        }

        [Category("Design")]
        public String Name { get; set; }

        [Category("Design")]
        public String Type { get; private set; }

        [Category("Design")]
        public Vector3 Position { get; set; }

        [Category("Design")]
        public Scale Scale { get; set; }

        [Category("Design")]
        public Vector3 FrontVector { get; set; }

    }
    public class Vector3 : SubCategory
    {
        public int x { get; set; }
        public int y { get; set; }
        public int z { get; set; }
    }
    public class Scale : SubCategory
    {
        public float x { get; set; }
        public float y { get; set; }
        public float z { get; set; }
    }
    
}
