#include <libxml/parser.h>
#include <libxml/tree.h>

class Modify_svg {
    private:

    public:

    void draw_map(std::list<std::tuple<double,double>> points){
        const char* input_svg_file = "./src/plot/world.svg";
        xmlDocPtr doc = xmlReadFile(input_svg_file, NULL, 0);
        if (doc == NULL) {
            // Error handling: Failed to parse the SVG file
            return 1;
        }

    }
}
