using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Reflection;

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

    public class Property
    {
        public Property()
        {
            Position = new Vector3();
            Scale = new Vector3();
            FrontVector = new Vector3();
        }
        [Browsable(false)]
        public Core.Node target { get; private set; }

        public void InitValue(Core.Node t)
        {
//             SetVisible("Name", false);
//             SetVisible("Type", false);
//             SetVisible("Position", false);
//             SetVisible("Scale", false);
//             SetVisible("FrontVector", false);

            target = t;

            Name = target.GetObjectName();
            Type = target.GetClassName();

            Position.Set(target.GetPosition());
            Scale.Set(target.GetScale());
            FrontVector.Set(target.GetFrontVector());
        }

        public void SetVisible(String PropertyName, bool value)
        {
            PropertyDescriptor descriptor = TypeDescriptor.GetProperties(this.GetType())[PropertyName];
            BrowsableAttribute attribute = (BrowsableAttribute)descriptor.Attributes[typeof(BrowsableAttribute)];
            FieldInfo Browsable = attribute.GetType().GetField("browsable", System.Reflection.BindingFlags.NonPublic | System.Reflection.BindingFlags.Instance);
            Browsable.SetValue(attribute, value);
        }
        [Category("Design"), Browsable(true)]
        public String Name { get; set; }

        [Category("Design"), Browsable(true)]
        public String Type { get; private set; }

        [Category("Design"), Browsable(true)]
        public Vector3 Position { get; set; }

        [Category("Design"), Browsable(true)]
        public Vector3 Scale { get; set; }

        [Category("Design"), Browsable(true)]
        public Vector3 FrontVector { get; set; }

    }
    public class Vector3 : SubCategory
    {
        public float x { get; set; }
        public float y { get; set; }
        public float z { get; set; }

        public void Set(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }
        public void Set(float[] v) { x = v[0]; y = v[1]; z = v[2]; }
    }    
}
