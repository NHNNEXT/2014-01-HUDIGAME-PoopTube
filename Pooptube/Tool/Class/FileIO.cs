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
                JsonObjectCollection obj = new JsonObjectCollection(child.GetClassName());
                JsonArrayCollection array;
                float[] arrayValue;

                // Object Name
                obj.Add(new JsonStringValue("name", child.GetObjectName()));

                /////////////// Positon ///////////////
                array = new JsonArrayCollection("position");
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
                array = new JsonArrayCollection("frontvector");
                arrayValue = child.GetFrontVector();
                array.Add(new JsonNumericValue(null, arrayValue[0]));
                array.Add(new JsonNumericValue(null, arrayValue[1]));
                array.Add(new JsonNumericValue(null, arrayValue[2]));

                obj.Add(array);


                // Add This Object
                collection.Add(obj);
            }
            System.IO.File.WriteAllText("test.json", collection.ToString());

            return true;
        }

        public static bool JsonInput(string filename, ref Core.Scene scene) { return true; }
    }
}
