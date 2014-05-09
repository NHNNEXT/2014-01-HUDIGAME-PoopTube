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
        private void KeyboardInput(object sender, KeyPressEventArgs e)
        {
            e.Handled = true;
            char input = Char.ToUpper(e.KeyChar);

            if (input == 'W') // FORWARD
            {
                //Camera.RotateFrontVectorY(-0.1f);
                //Camera.RotationY(-0.1f);
                Camera.Move(0.1f, 0.0f);
            }
            else if (input == 'S') // BACK
            {
                //Camera.RotateFrontVectorY(0.1f);
                //Camera.RotationY(0.1f);
                Camera.Move(-0.1f, 0.0f);
            }
            else if (input == 'A') // LEFT
            {
                Camera.Move(0.0f, 0.1f);
            }
            if (input == 'D') // RIGHT
            {
                Camera.Move(0.0f, -0.1f);
            }
            else
                e.Handled = false;
        }
    }
}
