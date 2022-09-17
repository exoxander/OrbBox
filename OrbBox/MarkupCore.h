#pragma once

#include "Utility.h";
#include "PhysicsBody.h";
#include <fstream>;
using std::fstream;
using std::shared_ptr;
using std::make_shared;
using std::string;


class MarkupCore {
public:
	shared_ptr<bodyList> startingBodies;
	shared_ptr<bodyList> existingBodies;
	string filepath;

public:MarkupCore() {
	startingBodies = nullptr;
	existingBodies = nullptr;
	filepath = "NO_PATH";
}
public:MarkupCore(shared_ptr<bodyList> _starting, shared_ptr<bodyList> _existing, string _path = "GameData.txt") {
	startingBodies = _starting;
	existingBodies = _existing;
	filepath = _path;
}

public:void markupReader() {

}
public: void markupWriter() {
	fstream writer(filepath);
	writer << "<readerinfo>\n\t{formatversion:1}\n\t{savecounter:1}\n</readerinfo>\n";
	shared_ptr<body> currentBody = startingBodies->head;

	while (currentBody != nullptr) {
		writer << "<object>\n";

		writer << "\t{objectid:" << currentBody->item->id << "}\n";
		writer << "\t{positionx:" << currentBody->item->position.x << "}\n";
		writer << "\t{positiony:" << currentBody->item->position.y << "}\n";
		writer << "\t{velocityx:" << currentBody->item->velocity.x << "}\n";
		writer << "\t{velocityy:" << currentBody->item->velocity.y << "}\n";
		writer << "\t{mass:" << currentBody->item->mass << "}\n";

		writer << "</object>\n";

		//every fucking time
		currentBody = currentBody->next;
	}

	writer.close();
}
};






/* SIMPLE MARKUP FORMAT SMF
	<name>			| object / structure opening
	</name>			| object / structure close
	{name:data}		| variable and its associated data
	//text			| a comment

	//game save format
	<readerinfo>
		{formatversion:3}
		{savecounter:2}
	</readerinfo>
	<savelist>
		<save>
			<gamedata>
				{saveid:001}
				{savename:thename}
				{totalbodies:7}
			</gamedata>
			<virtualbodies>
				<body>
					{id:0}
					{mass:120}
					{positionx:150.29304}
					{positiony:-384.332200}
					{velocityx:-1.349}
					{velocityy:.84648}

					//use later?
					<mesh>
						<vertexlist>
							<vertex>
								{id:0}
								{x:5.3}
								{y:1.2}
							</vertex>
						</vertexlist>
						<polygonlist>
							<poylgon>
								{id:0}
								{aid:0}
								{bid:1}
								{cid:2}
							</polygon>
						</polygonlist>
					</mesh>
				</body>
			</virtualbodies>
		</save>
	</savelist>
	*/
