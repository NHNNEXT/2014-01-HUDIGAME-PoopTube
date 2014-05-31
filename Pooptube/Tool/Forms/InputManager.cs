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
    public partial class MainForm
    {
//         [DllImport("user32.dll")]
//         public static extern int GetKeyboardState(byte[] keystate);
// 
// 
//         private void Form1_KeyDown(object sender, KeyEventArgs e)
//         {
//             byte[] keys = new byte[255];
// 
//             GetKeyboardState(keys);
// 
//             if (keys[(int)Keys.Up] == 129 && keys[(int)Keys.Right] == 129)
//             {
//                 Console.WriteLine("Up Arrow key and Right Arrow key down.");
//             }
//         }
        int a = 0, b = 0;
        private void KeyboardInput(object sender, KeyPressEventArgs e)
        {
            e.Handled = true;
            char input = Char.ToUpper(e.KeyChar);

            if (input == 'W') // FORWARD
            {
                //label1.Text = a.ToString();
                ++a;
                //Camera.RotateFrontVectorY(-0.1f);
                //Camera.RotationY(-0.1f);
                Camera.Move(0.3f, 0.0f);
            }
            if (input == 'S') // BACK
            {
                //label2.Text = b.ToString();
                ++b;
                //Camera.RotateFrontVectorY(0.1f);
                //Camera.RotationY(0.1f);
                Camera.Move(-0.3f, 0.0f);
            }
            if (input == 'A') // LEFT
            {
                Camera.Move(0.0f, 0.3f);
            }
            if (input == 'D') // RIGHT
            {
                Camera.Move(0.0f, -0.3f);
            }
            if (input == 'T') // RIGHT
            {
                Camera.RotationX(-0.1f);
            }
            if (input == 'G') // RIGHT
            {
                Camera.RotationX(0.1f);
            }
            if (input == 'F') // RIGHT
            {
                Camera.RotationY(-0.1f);
            }
            if (input == 'H') // RIGHT
            {
                Camera.RotationY(0.1f);
            }

            //else
            e.Handled = false;
        }
    }
}
