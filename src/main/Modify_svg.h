#include <iostream>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <list>
#include <tuple>

class Modify_svg {
    private:

        static void
        plot_points(std::list<std::tuple<double, double>> points,
                    double w, double h, xmlNodePtr root){

            for(auto point:points){
                double lat = std::get<0>(point);
                double lon = std::get<1>(point);

                std::tuple<double, double> t_tuple=convert_points(lat,lon, w,h);
                double lat2 = std::get<0>(t_tuple);
                double lon2 = std::get<1>(t_tuple);

                xmlNodePtr circleNode = xmlNewChild(root, NULL, BAD_CAST "circle", NULL);
                xmlNewProp(circleNode, BAD_CAST "cx", BAD_CAST std::to_string(lon2).c_str());
                xmlNewProp(circleNode, BAD_CAST "cy", BAD_CAST std::to_string(lat2).c_str());
                xmlNewProp(circleNode, BAD_CAST "r", BAD_CAST "1");
                if(lat == lon)
                    xmlNewProp(circleNode, BAD_CAST "fill", BAD_CAST "red");
                else
                    xmlNewProp(circleNode, BAD_CAST "fill", BAD_CAST "#007BA7");
            }
        }

        static void plot_path(std::list<std::tuple<double,double>> points_list,
            double w, double h, xmlNodePtr root){
            int size = points_list.size();
            std::tuple<double,double> points[size];
            size_t index = 0;
            for (std::tuple<double, double> value : points_list) {
                points[index] = value;
                ++index;
            }
            for(int i = 1; i < size; i++){
                double lat1 = std::get<0>(points[i-1]);
                double lon1 = std::get<1>(points[i-1]);

                double lat2 = std::get<0>(points[i]);
                double lon2 = std::get<1>(points[i]);

                std::tuple<double, double> t_tuple1=convert_points(lat1,lon1, w,h);
                double lat3 = std::get<0>(t_tuple1);
                double lon3 = std::get<1>(t_tuple1);
                std::tuple<double, double> t_tuple2=convert_points(lat2,lon2, w,h);
                double lat4 = std::get<0>(t_tuple2);
                double lon4 = std::get<1>(t_tuple2);


                xmlNodePtr lineNode = xmlNewChild(root, NULL, BAD_CAST "line", NULL);
                xmlNewProp(lineNode, BAD_CAST "x1", BAD_CAST std::to_string(lon3).c_str());
                xmlNewProp(lineNode, BAD_CAST "y1", BAD_CAST std::to_string(lat3).c_str());
                xmlNewProp(lineNode, BAD_CAST "x2", BAD_CAST std::to_string(lon4).c_str());
                xmlNewProp(lineNode, BAD_CAST "y2", BAD_CAST std::to_string(lat4).c_str());
                xmlNewProp(lineNode, BAD_CAST "stroke", BAD_CAST "red");
            }
        }

        static std::tuple<double, double>
        convert_points(double lat, double lon,double w, double h) {
            double svgX;
            double svgY;

            double minLon = -180.0;
            double maxLon = 180.0;
            double minLat = -90.0;
            double maxLat = 90.0;

            svgX = (lon - minLon) * (w / (maxLon - minLon));
            svgY = (maxLat - lat) * (h / (maxLat - minLat));
            return std::make_tuple(svgY, svgX);
        }



    public:
        static void draw_map(std::list<std::tuple<double, double>> points, const char* route) {
            const char* input_svg_file = "./data/plot/world5.svg";
            xmlDocPtr doc = xmlReadFile(input_svg_file, NULL, 0);
            if (doc == NULL) {
                std::cerr << "Error: Failed to parse the SVG file" << std::endl;
                return;
            }
            xmlNodePtr root = xmlDocGetRootElement(doc);
            xmlChar* widthAttr = xmlGetProp(root, BAD_CAST "width");
            xmlChar* heightAttr = xmlGetProp(root, BAD_CAST "height");
            double w = std::strtod(reinterpret_cast<const char*>(widthAttr), nullptr);
            double h = std::strtod(reinterpret_cast<const char*>(heightAttr), nullptr);

            plot_path(points, w,h,root);
            plot_points(points, w,h,root);

            const char* output_svg_file = route;
            xmlSaveFormatFile(output_svg_file, doc, 1);

            xmlFreeDoc(doc);
            xmlCleanupParser();
        }
};
