/**
 * Copyright 2020 Shusheng Shao <iblackangel@163.com>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <iostream>

// git clone https://github.com/dwd/rapidxml.git
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_print.hpp>
#include <rapidxml/rapidxml_utils.hpp>

void read_xml_file();
void read_xml();
void write_xml();

void read_xml_file()
{
	// must using try...catch
	try {
		std::string xmlfile = "rapidxml_file.xml";

		rapidxml::file<> file(xmlfile.c_str());

		// xml_document<>
		rapidxml::xml_document<> doc; 
		doc.parse<0>(file.data()); // parse xml string

		// xml_node<>
		rapidxml::xml_node<> *fetchTicket = doc.first_node("FetchTicket");
		rapidxml::xml_node<> *passenger = doc.first_node("FetchTicket")->first_node("Passenger");

		// count_children, count_attributes
		int childcount = rapidxml::count_children(passenger);
		int attrcount = rapidxml::count_attributes(passenger);

		printf("children of passenger is: %d\n", childcount);
		printf("passenger's attributes: %d\n", attrcount);

		// xml_attribute<>
		std::string idno = passenger->first_attribute("IDNO")->value();
		std::string name = passenger->first_attribute("Name")->value();

		printf("Passenger IDNO: '%s'\tName: '%s'\n",
			   idno.c_str(), name.c_str());
		printf("===========================================================\n");
		
		// get StartStation, ArriveStation, TrainNo, StartTime, ArriveTime
		for (rapidxml::xml_node<> *child = passenger->first_node("Ticket");
			 child; child = child->next_sibling()) {
			std::string starts = child->first_attribute("StartStation")->value();
			std::string arrives = child->first_attribute("ArriveStation")->value();
			std::string trainno = child->first_attribute("TrainNo")->value();
			std::string startt = child->first_attribute("StartTime")->value();
			std::string arrivet = child->first_attribute("ArriveTime")->value();

			printf("StartStation: '%s',\tArriveStation: '%s',\tTrainNo: '%s',\tStartTime: '%s',\tArriveTime: '%s'\n",
				   starts.c_str(), arrives.c_str(), trainno.c_str(),
				   startt.c_str(), arrivet.c_str());
		}

		// output string
		std::string outstr;
		rapidxml::print(std::back_inserter(outstr), doc);
		std::cout << outstr << std::endl;

	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
}

/*

  <?xml version="1.0" encoding="UTF-8"?> 
  <plugins>															   
    <plugin filename="&lt;builtin&gt;"><name>Internal</name><description>Objects and filters internal to Dia</description></plugin>
    <plugin filename="/usr/local/lib/dia/libjackson_objects.so"><name>Jackson</name><description>Jackson diagram</description></plugin>
  </plugins>

 */
void read_xml()
{
	// must using try...catch
	try {
		// xml string
		std::string xmlstr = "\
		<?xml version=\"1.0\" encoding=\"UTF-8\"?>\
		<plugins>\
		  <plugin filename=\"&lt;builtin&gt;\"><name>Internal</name><description>Objects and filters internal to Dia</description></plugin>\
		  <plugin filename=\"/usr/local/lib/dia/libjackson_objects.so\"><name>Jackson</name><description>Jackson diagram</description></plugin>\
		</plugins>";

		// Convert to char *
		char *chdata = const_cast<char *>(xmlstr.c_str());

		// xml_document<>
		rapidxml::xml_document<> doc;
		doc.parse<0>(chdata);

		printf("\n---------------------------------------------------------\n");

		// xml_node<>
		rapidxml::xml_node<> *xml = doc.first_node("plugins");

		// count_children, count_attributes
		int childcount = rapidxml::count_children(xml);
		int attrcount = rapidxml::count_attributes(xml);
		printf("childcount: %d, attrcount: %d\n", childcount, attrcount);
		
		for (rapidxml::xml_node<> *plugin = xml->first_node("plugin"); plugin; plugin = plugin->next_sibling()) {
			std::string filename = plugin->first_attribute("filename")->value();
			std::string name = plugin->first_node("name")->value();
			std::string desc = plugin->first_node("description")->value();
			printf("filename: '%s', name: '%s', description: '%s'\n",
				   filename.c_str(), name.c_str(), desc.c_str());
		}

		// output string
		std::string outstr;
		rapidxml::print(std::back_inserter(outstr), doc);
		std::cout << outstr;
		
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
}

void write_xml()
{
	// must be try...catch
	try {
		std::string strStart = "start";
		std::string strValue = "广州南";
		
		rapidxml::xml_document<> doc;
		rapidxml::xml_node<> *pi_node = doc.allocate_node(rapidxml::node_pi, "xml version=\"1.0\" encoding=\"UTF-8\"");
		doc.append_node(pi_node);
		rapidxml::xml_node<> *root = doc.allocate_node(rapidxml::node_element, "plugins");
		doc.append_node(root);
		rapidxml::xml_node<> *plugin = doc.allocate_node(rapidxml::node_element, "plugin");
		rapidxml::xml_attribute<> *start = doc.allocate_attribute(strStart.c_str(), strValue.c_str());
		plugin->append_attribute(start);
		root->append_node(plugin);

		rapidxml::xml_node<> *color = doc.allocate_node(rapidxml::node_element, "color");
		color->append_node(doc.allocate_node(rapidxml::node_element, "red", "0.1"));
		color->append_node(doc.allocate_node(rapidxml::node_element, "green", "0.2"));
		color->append_node(doc.allocate_node(rapidxml::node_element, "blue", "0.7"));
		root->append_node(color);


		printf("-------------------------------\nno indenting\n-------------------------------\n");
		// print no indenting
		std::string noindent;
		rapidxml::print(std::back_inserter(noindent), doc, rapidxml::print_no_indenting);
		std::cout << noindent << std::endl;

		printf("\n-------------------------------\npretty printing\n-------------------------------\n");
		// pretty print
		std::string outstr;
		rapidxml::print(std::back_inserter(outstr), doc);
		std::cout << outstr;

		std::ofstream out("/tmp/doc.xml");
		out << doc;
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
}

int main(int argc, char *argv[]) {
    read_xml_file();
    read_xml();
    write_xml();
    return 0;
}