using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Tool.Class;

namespace Tool
{
    public partial class MainForm : Form
    {
        Core.Application Application = new Core.Application();

        Core.Scene Scene = null;
        Core.Ground Ground = null;
        Core.SunLight SunLight = null;
        Core.Camera Camera = null;
        Core.Node SelectedNode = null;
        Property PropertyTable = new Property();

        public MainForm()
        {
            InitializeComponent();

            Application.init(this.ViewBox.Handle.ToInt32(), this.ViewBox.Size.Height, this.ViewBox.Size.Width);
            Scene = Core.Scene.Create();
            Ground = Core.Ground.Create();
            SunLight = Core.SunLight.Create();
            Camera = Core.Camera.Create();

            Camera.SetPosition(0, 5, -8);
            Camera.SetFrontVector(0.3f, -0.2f, 0.7f);

            Application.ChangeScene(Scene);
            Scene.AddChild(Ground);
            Scene.AddChild(SunLight);
            Scene.AddChild(Camera);

            Run();

            PropertyForm.SelectedObject = PropertyTable;
            ObjectListBox.Items.Clear();
            ObjectListBox.Items.AddRange(Scene.GetChildNameList().ToArray());
        }

        private async void Run()
        {
            while (true)
            {
                Application.Run();
                await Task.Delay(1);
            }
        }

        private void PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {
            PropertyGrid grid = (PropertyGrid)s;
            Property data = (Property)grid.SelectedObject;

            if (e.ChangedItem.Label == "Name")
            {
                Scene.ReplaceObjectName(data.target.GetObjectName(), data.Name);
                data.target.SetObjectName(data.Name);
            }
            else if (e.ChangedItem.Parent.Label == "Position") data.target.SetPosition(data.Position.x, data.Position.y, data.Position.z);
            else if (e.ChangedItem.Parent.Label == "Scale") data.target.SetScale(data.Scale.x, data.Scale.y, data.Scale.z);
            else if (e.ChangedItem.Parent.Label == "FrontVector")
            {
                data.target.SetFrontVector(data.FrontVector.x, data.FrontVector.y, data.FrontVector.z);
                data.FrontVector.Set(data.target.GetFrontVector());
            }
            grid.Refresh();
        }

        private void ChangeSeletedNode()
        {
            PropertyTable.InitValue(SelectedNode);

            PropertyForm.Refresh();

            ObjectListBox.Items.Clear();
            ObjectListBox.Items.AddRange(Scene.GetChildNameList().ToArray());
            if (SelectedNode == null) ObjectListBox.SelectedItem = null;
            else ObjectListBox.SelectedItem = SelectedNode.GetObjectName();
        }
        private void ViewBox_MouseDown(object sender, MouseEventArgs e)
        {
            if(e.Button == MouseButtons.Middle)
            {

            }
            else if(e.Button == MouseButtons.Right)
            {
                float[] pos = Ground.PICKGROUND(e.X, e.Y, 0.2f);

                SelectedNode = Core.Tiger.Create();
                SelectedNode.SetPosition(pos[0], pos[1], pos[2]);

                Scene.AddChild(SelectedNode);

                ChangeSeletedNode();
                
            }
            else if (e.Button == MouseButtons.Left)
            {
                SelectedNode = Scene.Pick(e.X, e.Y);

                if(SelectedNode != null)
                    ChangeSeletedNode();
            }
        }
    }
}
