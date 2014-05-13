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

            var settings = new Settings();
            settings.InitValue(Ground);
            settings.Position = new Vector3() { x = 0, y = 0, z = 0 };
            settings.Scale = new Scale() { x = 1, y = 1, z = 1 };
            settings.FrontVector = new Vector3() { x = 0, y = 0, z = 1 };

            PropertyForm.SelectedObject = settings;

            
            //PropertyForm.SelectedObject;
        }

        private async void Run()
        {
            while (true)
            {
//                 PropertyForm.Refresh();
//                 MyInformation.MyName = i.ToString();
//                 ++i;
                Application.Run();
                await Task.Delay(1);
            }
        }

        private void PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {
            PropertyGrid grid = (PropertyGrid)s;
            Settings data = (Settings)grid.SelectedObject;

            if (e.ChangedItem.Parent.Label == "Position")
                data.target.SetPosition(data.Position.x, data.Position.y, data.Position.z);

            else if (e.ChangedItem.Parent.Label == "Scale")
                data.target.SetScale(data.Scale.x, data.Scale.y, data.Scale.z);

            else if (e.ChangedItem.Parent.Label == "FrontVector")
            {
                data.target.SetFrontVector(data.FrontVector.x, data.FrontVector.y, data.FrontVector.z);
                data.FrontVector.Set(data.target.GetFrontVector());
                
            }
            grid.Refresh();
        }

        private void ViewBox_MouseDown(object sender, MouseEventArgs e)
        {
            label1.Text = e.X.ToString();
            label2.Text = e.Y.ToString();
            float []pos = Ground.PICKGROUND(e.X, e.Y, 0.2f);

            Core.Tiger t = Core.Tiger.Create();
            t.SetPosition(pos[0], pos[1], pos[2]);

            Scene.AddChild(t);
        }
    }
}
