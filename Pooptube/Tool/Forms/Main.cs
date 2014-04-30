using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Tool
{
    public partial class Form1 : Form
    {
        Core.Application Application = new Core.Application();
       
        public Form1()
        {
            InitializeComponent();

            Core.Scene Scene = null;
            Core.Ground Ground = null;
            Core.SunLight SunLight = null;

             
            Application.init(this.ViewBox.Handle.ToInt32(), this.Size.Height, this.Size.Width);
            Scene = new Core.Scene();
            Ground = new Core.Ground();
            SunLight = new Core.SunLight();

            Application.ChangeScene(Scene);
            Scene.AddChild(Ground);
            Scene.AddChild(SunLight);

            Run();
        }

        private async void Run()
        {
            while (true)
            {
                Application.Run();
                await Task.Delay(10);
            }
        }
    }
}
