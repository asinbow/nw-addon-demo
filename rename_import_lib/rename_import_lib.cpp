#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <pe_bliss.h>
#ifdef PE_BLISS_WINDOWS
#include "lib.h"
#endif

using namespace pe_bliss;

enum arg_status {
	none = 0,
	input_pe,
	output_pe,
	section_name,
	old_lib,
	new_lib
};

int main(int argc, char* argv[])
{
	if(argc <= 1)
	{
		std::cout << "Usage: rename_import_lib -i input_pe [-o output_pe] [-s section_name] -r old_lib new_lib [-r old_lib1 new_lib1]..." << std::endl;
		return 0;
	}

	std::string inputPE;
	std::string outputPE;
	std::string sectionName;
	typedef std::map<std::string, std::string> rename_map;
	rename_map renames;
	arg_status status = arg_status::none;
	std::string oldLib;

	for (int i = 1; i < argc; i++) {
		std::string arg = argv[i];
		//std::cout << "handle arg " << status << ", " << arg << std::endl;
		switch (status)
		{
		case none:
			if (arg == "-i") {
				status = arg_status::input_pe;
			}
			else if (arg == "-o") {
				status = arg_status::output_pe;
			}
			else if (arg == "-s") {
				status = arg_status::section_name;
			}
			else if (arg == "-r") {
				status = arg_status::old_lib;
			}
			else {
				std::cout << "invalid option " << arg << std::endl;
				return -1;
			}
			break;
		case input_pe:
			inputPE = arg;
			status = arg_status::none;
			break;
		case output_pe:
			outputPE = arg;
			status = arg_status::none;
			break;
		case section_name:
			sectionName = arg;
			status = arg_status::none;
			break;
		case old_lib:
			oldLib = arg;
			status = arg_status::new_lib;
			break;
		case new_lib:
			renames[oldLib] = arg;
			status = arg_status::none;
			break;
		default:
			break;
		}
	}

	if (inputPE.size() == 0) {
		std::cout << "Input PE not specified" << std::endl;
		return -1;
	}

	std::cout << "Input PE " << inputPE << std::endl;

	if (outputPE.size() == 0) {
		outputPE = inputPE;
		std::cout << "Output PE not specified, use " << outputPE << std::endl;
	}

	if (sectionName.size() == 0) {
		sectionName = "RENAMED";
		std::cout << "Section name not specified, use " << sectionName << std::endl;
	}

	if (renames.size() == 0) {
		std::cout << "No rename specified, quit" << std::endl;
		return -1;
	}
	for (rename_map::const_iterator ite = renames.begin(); ite != renames.end(); ite++) {
		//std::cout << "Will rename from " << ite->first << " to " << ite->second << std::endl;
	}

	std::ifstream inputStream(inputPE, std::ios::in | std::ios::binary);
	if(!inputStream)
	{
		std::cout << "Cannot open " << inputPE << std::endl;
		return -1;
	}

	try
	{
		pe_base image(pe_factory::create_pe(inputStream));
		inputStream.close();

		imported_functions_list imports(get_imported_functions(image));
		bool renamed = false;

		// rename import library
		for (imported_functions_list::iterator ite = imports.begin(); ite != imports.end(); ite++) {
			for (rename_map::const_iterator ite2 = renames.begin(); ite2 != renames.end(); ite2++) {
				if (ite->get_name() == ite2->first) {
					ite->set_name(ite2->second);
					renamed = true;
					std::cout << "Renamed import library from " << ite2->first << " to " << ite2->second << std::endl;
				}
			}
		}
		if (!renamed) {
			std::cout << "nothing renamed" << std::endl;
			return 0;
		}

		section new_imports;
		new_imports.get_raw_data().resize(1);
		new_imports.set_name("new_imp");
		new_imports.readable(true).writeable(true);
		section& attached_section = image.add_section(new_imports);

		import_rebuilder_settings settings(true, false);
		rebuild_imports(image, imports, attached_section, settings);

		std::ofstream outputStream(outputPE, std::ios::out | std::ios::binary | std::ios::trunc);
		if(!outputStream)
		{
			std::cout << "Cannot open " << outputPE << std::endl;
			return -1;
		}

		rebuild_pe(image, outputStream);

		std::cout << "PE was rebuilt and saved to " << outputPE << std::endl;
	}
	catch(const pe_exception& e)
	{
		std::cout << "Error: " << e.what() << std::endl;
		return -1;
	}

	return 0;
}
