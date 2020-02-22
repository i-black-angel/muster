/**
 * Copyright 2017 Shusheng Shao <iblackangel@163.com>
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
#include <stdio.h>
#include <fstream>

// git clone https://github.com/Tencent/rapidjson.git
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

// ----------VERY IMPORTENT-----------
// 
// should be compiled with
//
//         RAPIDJSON_HAS_STDSTRING
//
// macro

// g++ -DRAPIDJSON_HAS_STDSTRING

void json_read();
void json_write();

/*
{
    "hello": "world",
    "t": true ,
    "n": null,
    "i": 123,
    "pi": 3.1416,
    "a": [1, 2, 3, 4],
	"content" : {
		"project" : "rapidjson",
		"stars" : 10
	}
}
*/
void json_read()
{
	std::string json = "{\
    \"hello\": \"world\",\
    \"t\": true ,\
    \"n\": null,\
    \"i\": 123,\
    \"pi\": 3.1416,\
    \"a\": [1, 2, 3, 4],\
	\"content\":{\
	    \"project\" : \"rapidjson\",\
		\"stars\" : 10\
	}\
	}";

	// 1. create Document object, Parse json
	rapidjson::Document doc;
	rapidjson::ParseResult result = doc.Parse(json);
	if (!result) {
		printf("JSON parse error: %s (%lu)\n", rapidjson::GetParseError_En(result.Code()), result.Offset());
		return;
	}

	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> w(buffer);
	// rapidjson::Writer<rapidjson::StringBuffer> w(buffer);
	doc.Accept(w);

	printf("%s\n", buffer.GetString());
	
    // access members
	for (rapidjson::Value::ConstMemberIterator cit = doc.MemberBegin();
		 cit != doc.MemberEnd(); ++cit) {
		printf("member: \"%s\"\n", cit->name.GetString());
	}
	printf("\n");

	rapidjson::Value::ConstMemberIterator itr;

	// 2. read string
	itr = doc.FindMember("hello");
	if (itr != doc.MemberEnd() && itr->value.IsString())
		printf("\"hello\": \"%s\"\n", itr->value.GetString());

	// 3. read bool
	itr = doc.FindMember("t");
	if (itr != doc.MemberEnd() && itr->value.IsBool())
		printf("\"t\": %s\n", itr->value.GetBool() ? "true" : "false");

	// 4. read null
	itr = doc.FindMember("n");
	if (itr != doc.MemberEnd())
		printf("\"n\": %s\n", itr->value.IsNull() ? "null" : "?");

	// 5. read int
	itr = doc.FindMember("i");
	if (itr != doc.MemberEnd() && itr->value.IsInt())
		printf("\"i\": %d\n", itr->value.GetInt());

	// 6. read double
	itr = doc.FindMember("pi");
	if (itr != doc.MemberEnd() && itr->value.IsDouble())
		printf("\"pi\": %g\n", itr->value.GetDouble());

	// 7. read array
	itr = doc.FindMember("a");
	if (itr != doc.MemberEnd() && itr->value.IsArray()) {
		const rapidjson::Value &a = itr->value;
		for (rapidjson::SizeType i = 0; i < a.Size(); ++i) {
			printf("a[%d] = %d\n", i, a[i].GetInt());
		}
	}

	// 8. read object
	itr = doc.FindMember("content");
	if (itr != doc.MemberEnd() && itr->value.IsObject()) {
		const rapidjson::Value &cnt = itr->value;
		
		rapidjson::Value::ConstMemberIterator citr;
		citr = cnt.FindMember("project");
		if (citr != cnt.MemberEnd() && citr->value.IsString())
			printf("\"content.project\": \"%s\"\n", citr->value.GetString());
		
		citr = cnt.FindMember("stars");
		if (citr != cnt.MemberEnd() && citr->value.IsInt())
			printf("\"content.stars\": %d\n", citr->value.GetInt());
	}
}

void json_write(const std::string &url)
{
	rapidjson::Document doc;
	rapidjson::Document::AllocatorType &allocator =  doc.GetAllocator();
	doc.SetObject();

	// 1. add basic type value
	rapidjson::Value urlv(rapidjson::kStringType);
	urlv.SetString(url, allocator);
	doc.AddMember(urlv, "https://www.google.com", allocator);
	std::string strvalue = "blackangel";
	doc.AddMember("username", strvalue, allocator);
    doc.AddMember("user", "spiderman", allocator);
	doc.AddMember("price", 123, allocator);
	doc.AddMember("yesorno", true, allocator);
	doc.AddMember("height", 1.68, allocator);
	rapidjson::Value nullValue(rapidjson::kNullType);
	doc.AddMember("nullptr", nullValue, allocator);

	// 2. add array
	rapidjson::Value terminals(rapidjson::kArrayType);

	std::string value = "good";
	rapidjson::Value item(rapidjson::kStringType);
	item.SetString(value, allocator);
	terminals.PushBack(item, allocator);
	terminals.PushBack("bad", allocator);

	// 3. add object
	std::string name = "zhangshan";
	rapidjson::Value nameValue(rapidjson::kStringType);
	nameValue.SetString(name, allocator);
	
	rapidjson::Value content(rapidjson::kObjectType);
	content.AddMember("id", 1005, allocator);
	content.AddMember("name", name, allocator);

	doc.AddMember("content", content, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> w(buffer);
	// rapidjson::Writer<rapidjson::StringBuffer> w(buffer);
	doc.Accept(w);

	printf("%s\n", buffer.GetString());

	std::ofstream out("/tmp/jwrite.json");
	out << buffer.GetString();
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");
    json_read();
	json_write("success_url");
    return 0;
}
