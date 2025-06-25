#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "pugixml.hpp"

static int s_nodeID {0};
static int s_surfID {1};
static int s_cellID {1};

template <typename T>
void set_attribute(std::string_view name, const T& value, pugi::xml_node& node)
{
    node.append_attribute(name);
    pugi::xml_attribute attribute = node.attribute(name);
    attribute.set_value(value);
}

void make_surface(int nodeID, int surfID, const std::string & coeffs, const std::string & type, pugi::xml_node & node )
{
    node.append_child(std::to_string(nodeID));
    pugi::xml_node surf = node.child(std::to_string(nodeID));
    ++nodeID;
    surf.set_name("surface");
    set_attribute("coeffs", coeffs, node);
    set_attribute("id", surfID++, node);
    set_attribute("type", type, node);
}

int main()
{
    //input data
    std::vector<double> cyl_radii = {0.46955, 0.4791, 0.54640};
    double height {300};
    double pitch {1.44270};
    std::vector<std::string_view> materials = {"2","void", "1", "3"};
    //

    pugi::xml_document root;
    root.append_child("geometry");
    pugi::xml_node geometry = root.child("geometry");

    //Defining surfaces
    //creating cylinder surfaces (ids 1-3)
    for(std::size_t i{0}; i<cyl_radii.size(); ++i)
    {
        geometry.append_child(std::to_string(s_nodeID));
        pugi::xml_node cyl = geometry.child(std::to_string(s_nodeID));
        ++s_nodeID;
        cyl.set_name("surface");
        set_attribute("coeffs", "0.0 0.0 " + std::to_string(cyl_radii.at(i)), cyl);
        set_attribute("id", s_surfID++, cyl);
        set_attribute("type", "z-cylinder", cyl);
    }
    //creating sides (ids 4-7)
    geometry.append_child(std::to_string(s_nodeID));
    pugi::xml_node left = geometry.child(std::to_string(s_nodeID));
    ++s_nodeID;
    left.set_name("surface");
    set_attribute("boundary", "reflective", left);
    set_attribute("coeffs", std::to_string(-pitch/2), left);
    set_attribute("id", s_surfID++, left);
    set_attribute("type", "x-plane", left);

    geometry.append_child(std::to_string(s_nodeID));
    pugi::xml_node right = geometry.child(std::to_string(s_nodeID));
    ++s_nodeID;
    right.set_name("surface");
    set_attribute("boundary", "reflective", right);
    set_attribute("coeffs", std::to_string(pitch/2), right);
    set_attribute("id", s_surfID++, right);
    set_attribute("type", "x-plane", right);

    geometry.append_child(std::to_string(s_nodeID));
    pugi::xml_node back = geometry.child(std::to_string(s_nodeID));
    ++s_nodeID;
    back.set_name("surface");
    set_attribute("boundary", "reflective", back);
    set_attribute("coeffs", std::to_string(-pitch/2), back);
    set_attribute("id", s_surfID++, back);
    set_attribute("type", "y-plane", back);

    geometry.append_child(std::to_string(s_nodeID));
    pugi::xml_node front = geometry.child(std::to_string(s_nodeID));
    ++s_nodeID;
    front.set_name("surface");
    set_attribute("boundary", "reflective", front);
    set_attribute("coeffs", std::to_string(pitch/2), front);
    set_attribute("id", s_surfID++, front);
    set_attribute("type", "y-plane", front);

    //creating top and bottom (ids 8 and 9)
    geometry.append_child(std::to_string(s_nodeID));
    pugi::xml_node top = geometry.child(std::to_string(s_nodeID));
    ++s_nodeID;
    top.set_name("surface");
    set_attribute("boundary", "vacuum", top);
    set_attribute("coeffs", std::to_string(height/2), top);
    set_attribute("id", s_surfID++, top);
    set_attribute("type", "z-plane", top);

    geometry.append_child(std::to_string(s_nodeID));
    pugi::xml_node bottom = geometry.child(std::to_string(s_nodeID));
    ++s_nodeID;
    bottom.set_name("surface");
    set_attribute("boundary", "vacuum", bottom);
    set_attribute("coeffs", std::to_string(-height/2), bottom);
    set_attribute("id", s_surfID++, bottom);
    set_attribute("type", "z-plane", bottom);

    //Defining cells
    //Define cylinder regions
    std::string fuel_region{"-1"};
    std::vector<std::string> regions{fuel_region};
    for(std::size_t i{1}; i<cyl_radii.size(); ++i)
    {
        std::string region{std::to_string(i)+ " -" + std::to_string(i+1)};
        regions.push_back(region);
    }
    std::string coolant_region{std::to_string(cyl_radii.size())+ " "+ std::to_string(cyl_radii.size()+1)+ " -"+ std::to_string(cyl_radii.size()+2)+ " "+ std::to_string(cyl_radii.size()+3)+ " -"+ std::to_string(cyl_radii.size()+4)};
    regions.push_back(coolant_region);

    for(std::size_t i{0};i <materials.size();++i)
    {
        geometry.append_child(std::to_string(s_nodeID));
        pugi::xml_node cell = geometry.child(std::to_string(s_nodeID));
        ++s_nodeID;
        cell.set_name("cell");
        set_attribute("id", s_cellID++, cell);
        set_attribute("material",materials.at(i), cell); 
        set_attribute("region",regions.at(i),cell);
        set_attribute("universe", 1,cell);
    }

    //Adding top and bottom (advanced extruder generator)
    for(pugi::xml_node cell = geometry.child("cell"); cell; cell = cell.next_sibling("cell"))
    {
        pugi::xml_attribute attrib = cell.attribute("region");
        std::string region = attrib.value();
        region = region + " -" + std::to_string(cyl_radii.size()+5) + " " + std::to_string(cyl_radii.size()+6);
        attrib.set_value(region);
    }

    std::cout << "Saving result: " << root.save_file("\\\\wsl.localhost\\Ubuntu\\home\\dimit\\openmc\\build\\geometry.xml") << std::endl;
    
    return 0;
}

