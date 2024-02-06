#pragma once
#include <string>
#include "nlohmann/json.hpp"

// nlohmann::json 객체를 만드는 표현식인 nlohmann::json(container)이 nlohmann::json으로 변환이 가능해야함
template<typename T>
concept SerializeableContainer = requires(const T & container) {
	{nlohmann::json(container)} -> std::convertible_to<nlohmann::json>;
};


template<typename Container>
std::string SerializeContainer(const Container& myContainer, std::wstring& filename) requires SerializeableContainer<Container>
{
	nlohmann::json jsonObject = myContainer;

	return jsonObject.dump(2);
}

template <typename Container>
Container DeserializeContainerFromFile(const std::wstring& filename) requires SerializeableContainer<Container> {
	std::ifstream inputFile(filename);
	nlohmann::json jsonObject;
	inputFile >> jsonObject;
	inputFile.close();

	return jsonObject.get<Container>();
}

// 내가 하고 싶은건 GameObject vector를 넘겼을 때 
// 그 오브젝트들이 가지고 있는 Component Class들도 직력화가 되고
// 그 Component들이 가지고 있는 멤버 변수 값들도 직렬화가 한번에 촤르륵 되게하고싶음 

// GameObject vector를 넘기게 되면 포인터를 serialize해야되기 때문에 힘듬
// Entity 따로 serialize하고 Component도 따로 serialize한 후 
// 초기화 해줄 때 Entity의 ID값에 따라 Component넣어주는 방법
