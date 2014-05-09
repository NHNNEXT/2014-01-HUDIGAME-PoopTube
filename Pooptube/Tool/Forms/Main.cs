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

            
             
            Application.init(this.ViewBox.Handle.ToInt32(), this.Size.Height, this.Size.Width);
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
            settings.InitValue(Scene);
            //settings.Position = new Vector3() { x = 1, y = 2, z = 3 };
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
                await Task.Delay(10);
            }
        }
    }

}
