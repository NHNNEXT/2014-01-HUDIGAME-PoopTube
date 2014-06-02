using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Json;

namespace Tool.Class
{
    public class FileIO
    {
        public static bool JsonOutput(string filename, List<Core.Node> ChildList) 
        {
            JsonUtility.GenerateIndentedJsonText = true;
            JsonObjectCollection collection = new JsonObjectCollection("Map");

            foreach (var child in ChildList)
            {
                JsonObjectCollection obj = new JsonObjectCollection();
                JsonArrayCollection array;
                float[] arrayValue;

                // class Name
                obj.Add(new JsonStringValue("Class", child.GetClassName()));
                // Object Name
                obj.Add(new JsonStringValue("Name", child.GetObjectName()));

                /////////////// Positon ///////////////
                array = new JsonArrayCollection("Position");
                arrayValue = child.GetPosition();
                array.Add(new JsonNumericValue(null, arrayValue[0]));
                array.Add(new JsonNumericValue(null, arrayValue[1]));
                array.Add(new JsonNumericValue(null, arrayValue[2]));

                obj.Add(array);

                //////////////// Scale ////////////////
                array = new JsonArrayCollection("Scale");
                arrayValue = child.GetScale();
                array.Add(new JsonNumericValue(null, arrayValue[0]));
                array.Add(new JsonNumericValue(null, arrayValue[1]));
                array.Add(new JsonNumericValue(null, arrayValue[2]));

                obj.Add(array);

                ///////////// Front Vector ////////////
                array = new JsonArrayCollection("FrontVector");
                arrayValue = child.GetFrontVector();
                array.Add(new JsonNumericValue(null, arrayValue[0]));
                array.Add(new JsonNumericValue(null, arrayValue[1]));
                array.Add(new JsonNumericValue(null, arrayValue[2]));

                obj.Add(array);


                // Add This Object
                collection.Add(obj);
            }
            JsonObjectCollection root = new JsonObjectCollection();
            root.Add(collection);
            System.IO.File.WriteAllText("test.json", root.ToString());

            return true;
        }

        public static bool JsonInput(string filename, ref Core.Scene scene) { return true; }
    }
}
