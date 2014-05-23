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
using System.Net.Json;

namespace Tool
{
    public partial class MainForm : Form
    {
        //JavaScriptSerializer
        Core.Application Application = new Core.Application();

        Core.Scene Scene = null;
        Core.Ground Ground = null;
        Core.SunLight SunLight = null;
        Core.Camera Camera = null;
        Core.Node SelectedNode = null;
        int SelectedObjectType = 0;
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
            if(e.Button == MouseButtons.Right)
            {
                float[] pos = Ground.GetIntersectPosThis(e.X, e.Y);
                switch(SelectedObjectType)
                {
                    case 0 :
                        SelectedNode = Core.WCreature.Create();
                        break;
                    case 1 :
                        SelectedNode = Core.WLightOrb.Create();
                        break;
                    case 2 :
                        SelectedNode = Core.WTree.Create();
                        break;
                    case 3 :
                        //Ground.AddHeight(e.X, e.Y, 0.2f);
                        return;
                    case 4 :
                        //Ground.AddHeight(e.X, e.Y, -0.2f);
                        return;
                }

                SelectedNode.SetPosition(pos[0], pos[1], pos[2]);
                Scene.AddChild(SelectedNode);
                ChangeSeletedNode();
                 
                
//                 float[] pos = Ground.PICKGROUND(e.X, e.Y, 0.2f);
// 
//                 //SelectedNode = Core.WCreature.Create();
//                 SelectedNode = Core.WLightOrb.Create();
//                 SelectedNode.SetPosition(pos[0], pos[1], pos[2]);
// 
//                 Scene.AddChild(SelectedNode);
// 
//                 ChangeSeletedNode();
                
            }
            else if (e.Button == MouseButtons.Left)
            {
                SelectedNode = Scene.Pick(e.X, e.Y);

                if(SelectedNode != null)
                    ChangeSeletedNode();
            }
        }

        private void SaveMapFile(object sender, EventArgs e)
        {
            SaveFileDialog saveFileDialog1 = new SaveFileDialog();

            saveFileDialog1.Filter = "JSON map files (*.json)|*.json";
            saveFileDialog1.FilterIndex = 1;
            saveFileDialog1.RestoreDirectory = true;
            saveFileDialog1.InitialDirectory = System.IO.Directory.GetCurrentDirectory();

            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                String fileName = saveFileDialog1.FileName;
                

            }
        }

        private void LoadMapFile(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog1 = new OpenFileDialog();

            openFileDialog1.Filter = "JSON map files (*.json)|*.json";
            openFileDialog1.FilterIndex = 1;
            openFileDialog1.RestoreDirectory = true;
            openFileDialog1.InitialDirectory = System.IO.Directory.GetCurrentDirectory();

            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                //OpenCSMFile(openFileDialog1.FileName);
            }
        }

        private void NewMapFile(object sender, EventArgs e)
        {

        }

        private void ExitProgram(object sender, EventArgs e)
        {
            Environment.Exit(0);
        }

        private void ChangedObjectType(object sender, EventArgs e)
        {
            RadioButton target = ((RadioButton)sender);
            if(target.Checked == true)
            {
                if (target.Text == "Creature") SelectedObjectType = 0;
                else if (target.Text == "Light Object") SelectedObjectType = 1;
                else if (target.Text == "Tree") SelectedObjectType = 2;
                else if (target.Text == "Up") SelectedObjectType = 3;
                else if (target.Text == "Down") SelectedObjectType = 4;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            const string jsonText =
            "{"+
            " \"FirstValue\": 1.1,"+
            " \"SecondValue\": \"some text\"," +
            " \"TrueValue\": true" +
            "}";

            // 1. parse sample
            richTextBox1.AppendText("\n");
            richTextBox1.AppendText("Source data:\n");
            richTextBox1.AppendText(jsonText);
            richTextBox1.AppendText("\n");

            JsonTextParser parser = new JsonTextParser();
            JsonObject obj = parser.Parse(jsonText);

            richTextBox1.AppendText("\n");
            richTextBox1.AppendText("Parsed data with indentation in JSON data format:\n");
            richTextBox1.AppendText(obj.ToString());
            richTextBox1.AppendText("\n");

            JsonUtility.GenerateIndentedJsonText = false;

            richTextBox1.AppendText("\n");
            richTextBox1.AppendText("Parsed data without indentation in JSON data format:\n");
            richTextBox1.AppendText(obj.ToString());
            richTextBox1.AppendText("\n");

            // enumerate values in json object
            richTextBox1.AppendText("\n");
            richTextBox1.AppendText("Parsed object contains these nested fields:\n");

            foreach (JsonObject field in obj as JsonObjectCollection)
            {
                string name = field.Name;
                string value = string.Empty;
                string type = field.GetValue().GetType().Name;

                // try to get value.
                switch(type)
                {
                    case "String":
                        value = (string)field.GetValue();
                        break;
                    case "Double":
                        value = field.GetValue().ToString();
                        break;
                    case "Boolean":
                        value = field.GetValue().ToString();
                        break;
                    default:
                        // in this sample we'll not parse nested arrays or objects.
                        throw new NotSupportedException();
                }
                richTextBox1.AppendText(String.Format("{0} {1} {2}",
                name.PadLeft(15), type.PadLeft(10), value.PadLeft(15)));
            }
            richTextBox1.AppendText("\n");

            // 2. generate sample
            richTextBox1.AppendText("\n");

            // root object
            JsonObjectCollection collection = new JsonObjectCollection();

            // nested values
            collection.Add(new JsonStringValue("FirstName", "Pavel"));
            collection.Add(new JsonStringValue("LastName", "Lazureykis"));
            collection.Add(new JsonNumericValue("Age", 23));
            collection.Add(new JsonStringValue("Email", "me@somewhere.com"));
            collection.Add(new JsonBooleanValue("HideEmail", true));

            richTextBox1.AppendText("Generated object:\n");
            JsonUtility.GenerateIndentedJsonText = true;
            richTextBox1.AppendText(collection.ToString());
            richTextBox1.AppendText("\n");
            // 3. generate own library for working with own custom json objects
            /// 
            /// Note that generator in this pre-release version of library supports
            /// only JsonObjectCollection in root level ({...}) and only simple
            /// value types can be nested. Not arrays or other objects.
            /// Also names of nested values cannot contain spaces or starts with
            /// numeric symbols. They must comply with C# variable declaration rules.
            /// 
//             JsonGenerator generator = new JsonGenerator();
//             generator.GenerateLibrary("Person", collection, @"C:\");
            richTextBox1.AppendText("\n");

            System.IO.File.WriteAllText("test.json", collection.ToString());
        }
    }
}
