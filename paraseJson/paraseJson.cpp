// paraseJson.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iconv.h>


int test_rapidjson_parse()
{
	const char* file_name = "in.json";

	std::ifstream in(file_name);
	if (!in.is_open()) {
		fprintf(stderr, "fail to read json file: %s\n", file_name);
		return -1;
	}

	std::string json_content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	in.close();

	rapidjson::Document dom;
	if (!dom.Parse(json_content.c_str()).HasParseError()) {
		if (dom.HasMember("name") && dom["name"].IsString()) {
			fprintf(stdout, "name: %s\n", dom["name"].GetString());
		}

		if (dom.HasMember("address") && dom["address"].IsString()) {
			fprintf(stdout, "address: %s\n", (dom["address"].GetString()));
		}

		if (dom.HasMember("age") && dom["age"].IsInt()) {
			fprintf(stdout, "age: %d\n", dom["age"].GetInt());
		}

		const char* tmp = "xxx";
		if (!dom.HasMember(tmp)) {
			fprintf(stdout, "Warning: it has no member: %s\n", tmp);
		}

		if (dom.HasMember("value1") && dom["value1"].IsArray()) {
			const rapidjson::Value& arr = dom["value1"];
			for (int i = 0; i < arr.Size(); ++i) {
				const rapidjson::Value& tmp = arr[i];

				fprintf(stdout, "value1:\ni = %d:", i);
				for (int j = 0; j < tmp.Size(); ++j) {
					if (tmp[j].IsInt())
						fprintf(stdout, "%d, ", tmp[j].GetInt());
					if (tmp[j].IsFloat())
						fprintf(stdout, "%.1f, ", tmp[j].GetFloat());
				}
				fprintf(stdout, "\n");
			}
		}

		if (dom.HasMember("value2") && dom["value2"].IsArray()) {
			const rapidjson::Value& arr = dom["value2"];

			fprintf(stdout, "value2: ");
			for (int i = 0; i < arr.Size(); ++i) {
				fprintf(stdout, "%.2f, ", arr[i].GetFloat());
			}
			fprintf(stdout, "\n");
		}

		if (dom.HasMember("bei_jing") && dom["bei_jing"].IsObject()) {
			const rapidjson::Value& obj = dom["bei_jing"];

			if (obj.HasMember("address") && obj["address"].IsString()) {
				fprintf(stdout, "address: %s\n", (obj["address"].GetString()));
			}

			if (obj.HasMember("car") && obj["car"].IsBool()) {
				fprintf(stdout, "car: %d\n", obj["car"].GetBool());
			}

			if (obj.HasMember("cat") && obj["cat"].IsBool()) {
				fprintf(stdout, "cat: %d\n", obj["cat"].GetBool());
			}
		}

		if (dom.HasMember("shan_dong") && dom["shan_dong"].IsObject()) {
			const rapidjson::Value& obj = dom["shan_dong"];

			if (obj.HasMember("address") && obj["address"].IsString()) {
				fprintf(stdout, "address: %s\n", (obj["address"].GetString()));
			}

			if (obj.HasMember("value1") && obj["value1"].IsArray()) {
				const rapidjson::Value& arr = obj["value1"];

				fprintf(stdout, "value1: ");
				for (int i = 0; i < arr.Size(); ++i) {
					fprintf(stdout, "%s, ", arr[i].GetString());
				}
				fprintf(stdout, "\n");
			}
		}
	}
	else {
		fprintf(stderr, "fail to parse json file: %s\n", file_name);
		return -1;
	}

	// iter json
	/*
	enum Type {
		kNullType = 0,      //!< null
		kFalseType = 1,     //!< false
		kTrueType = 2,      //!< true
		kObjectType = 3,    //!< object
		kArrayType = 4,     //!< array
		kStringType = 5,    //!< string
		kNumberType = 6     //!< number
	};
	*/
	for (rapidjson::Value::ConstMemberIterator iter = dom.MemberBegin(); iter != dom.MemberEnd(); ++iter) {
		fprintf(stdout, "iter json name: %s, type: %d\n", iter->name.GetString(), iter->value.GetType());
	}

	return 0;
}

int test_rapidjson_write()
{
	rapidjson::StringBuffer buf;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buf); // it can word wrap

	writer.StartObject();

	writer.Key("name"); writer.String("spring");
	writer.Key("address"); writer.String("北京");
	writer.Key("age"); writer.Int(30);

	writer.Key("value1");
	writer.StartArray();
	writer.StartArray();
	writer.Double(23); writer.Double(43); writer.Double(-2.3); writer.Double(6.7);
	writer.Double(90);
	writer.EndArray();

	writer.StartArray();
	writer.Int(-9); writer.Int(-19); writer.Int(10); writer.Int(2);
	writer.EndArray();

	writer.StartArray();
	writer.Int(-5); writer.Int(-55);
	writer.EndArray();
	writer.EndArray();

	writer.Key("value2");
	writer.StartArray();
	writer.Double(13.3); writer.Double(1.9); writer.Double(2.10);
	writer.EndArray();

	writer.Key("bei_jing");
	writer.StartObject();
	writer.Key("address"); writer.String("海淀");
	writer.Key("car"); writer.Bool(false);
	writer.Key("cat"); writer.Bool(true);
	writer.EndObject();

	writer.Key("shan_dong");
	writer.StartObject();
	writer.Key("address"); writer.String("济南");
	writer.Key("value1");
	writer.StartArray();
	writer.Key("ji_nan"); writer.String("趵突泉");
	writer.Key("tai_an"); writer.String("泰山");
	writer.EndArray();
	writer.EndObject();

	writer.EndObject();

	const char* json_content = buf.GetString();
	fprintf(stdout, "json content: %s\n", json_content);

	const char* file_name = "out.json";

	std::ofstream outfile;
	outfile.open(file_name);
	if (!outfile.is_open()) {
		fprintf(stderr, "fail to open file to write: %s\n", file_name);
		return -1;
	}

	outfile << json_content << std::endl;
	outfile.close();

	return 0;
}

int main()
{
	test_rapidjson_write();		//生成json文件
	//test_rapidjson_parse();	//解析json内容
}