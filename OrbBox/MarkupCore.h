#pragma once

#include "Utility.h"
#include "PhysicsBody.h"
#include <fstream>
using std::fstream;
using std::shared_ptr;
using std::make_shared;
using std::string;
//https://en.cppreference.com/w/cpp/filesystem

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
public:mesh meshReader() {
	return mesh();
}
public:void meshWriter(mesh _item) {

}
public:void markupReader() {

}
public: void markupWriter() {
	if (filepath != "NO_PATH") {
		fstream writer(filepath);
		writer << "<readerInfo>\n\t{formatVersion:1}\n\t{saveCounter:1}\n</readerInfo>\n";
		shared_ptr<body> currentBody = startingBodies->head;
		shared_ptr<vertex> currentVertex;
		shared_ptr<polygon> currentPolygon;

		while (currentBody != nullptr) {
			currentVertex = currentBody->item->bodyMesh.vertexList;
			currentPolygon = currentBody->item->bodyMesh.polygonList;
			writer << "<object>\n";
			//basic object data
			writer << "\t{objectID:" << currentBody->item->id << "}\n";
			writer << "\t{positionX:" << currentBody->item->position.x << "}\n";
			writer << "\t{positionY:" << currentBody->item->position.y << "}\n";
			writer << "\t{velocityX:" << currentBody->item->velocity.x << "}\n";
			writer << "\t{velocityY:" << currentBody->item->velocity.y << "}\n";
			writer << "\t{mass:" << currentBody->item->mass << "}\n";
			//mesh data
			writer << "\t<mesh>\n";
			//vertecies
			writer << "\t\t<vertexList>\n";
			while (currentVertex != nullptr) {
				writer << "\t\t\t<vertex>\n";
				writer << "\t\t\t\t{id:" << currentVertex->id << "}\n";
				writer << "\t\t\t\t{positionX:" << currentVertex->position.x << "}\n";
				writer << "\t\t\t\t{positionY:" << currentVertex->position.y << "}\n";
				writer << "\t\t\t</vertex>\n";

				currentVertex = currentVertex->next;
			}
			writer << "\t\t</vertexList>\n";
			//polygons
			writer << "\t\t<polygonList>\n";
			while (currentPolygon != nullptr) {
				writer << "\t\t\t<polygon>\n";
				writer << "\t\t\t\t{id:" << currentPolygon->id << "}\n";
				writer << "\t\t\t\t{vertexA:" << currentPolygon->a->id << "}\n";
				writer << "\t\t\t\t{vertexB:" << currentPolygon->b->id << "}\n";
				writer << "\t\t\t\t{vertexC:" << currentPolygon->c->id << "}\n";
				writer << "\t\t\t</polygon>\n";

				currentPolygon = currentPolygon->next;
			}
			writer << "\t\t</polygonList>\n";
			writer << "\t</mesh>\n";
			writer << "</object>\n";

			//every fucking time
			currentBody = currentBody->next;
		}

		writer.close();
	}
	else {
		//cout << "unable to save: path is \"NO_PATH\"\n";
	}
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
