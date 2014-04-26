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
        Core.Application aa = new Core.Application();

        public Form1()
        {
            InitializeComponent();

            aa.init(this.ViewBox.Handle.ToInt32(), this.Size.Height, this.Size.Width);

            Render();
            
        }

        private async void Render()
        {
            while (true)
            {
                aa.Run();
                await Task.Delay(10);
            }
        }

        private void ViewBox_Click(object sender, EventArgs e)
        {

        }
    }
}
