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
            Core.Camera Camera = null;

            //MessageBox.Show(Scene.TEST().ToString() + Ground.TEST().ToString() + SunLight.TEST().ToString() + Camera.TEST().ToString());
            //Core.ThirdPersonCamera bb;
             
            Application.init(this.ViewBox.Handle.ToInt32(), this.Size.Height, this.Size.Width);
            Scene = Core.Scene.Create();
            Ground = Core.Ground.Create();
            SunLight = Core.SunLight.Create();
            Camera = Core.Camera.Create();

           // MessageBox.Show(Scene.TEST().ToString() + Ground.TEST().ToString() + SunLight.TEST().ToString() + Camera.TEST().ToString());
//             Scene = new Core.Scene();
//             Ground = new Core.Ground();
//             SunLight = new Core.SunLight();
//             Camera = new Core.Camera();

//             Core.SunLight test1 = new Core.SunLight();
//             Core.SunLight test2 = Core.SunLight.Create();
//             Core.SunLight test3 = Core.SunLight.TCreate();
// 
//             test3.SetPosition(1, 1, 1);
//            // test2 = test3;
//             test2.SetPosition(1, 1, 1);
//             test1.SetPosition(1, 1, 1);

            //SunLight.TEST();
//             SunLight.SetRange(1);
//             SunLight.TEST2(2);
//             SunLight.BB(2, 2, 2);
//             SunLight.AA(2, 2, 2);

            Camera.SetPosition(0, 5, -8);
            //Camera.SetLookAtPt(0, 0, 0);
            Camera.SetFrontVector(0.3f, -0.2f, 0.7f);

//              mCamera_2->SetPosition(0, 5, -8);
//              mCamera_2->SetLookAtPt(D3DXVECTOR3(0, 0, 0));
//              mCamera_2->SetFrontVector(0, 0, 1);

            Application.ChangeScene(Scene);
            Scene.AddChild(Ground);
            Scene.AddChild(SunLight);
            Scene.AddChild(Camera);
            
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
