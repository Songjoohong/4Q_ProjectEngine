#pragma once
#include <string>
#include "nlohmann/json.hpp"

// nlohmann::json 객체를 만드는 표현식인 nlohmann::json(container)이 nlohmann::json으로 변환이 가능해야함
template<typename T>
concept SerializeableContainer = requires(const T & container) {
	{nlohmann::json(container)} -> std::convertible_to<nlohmann::json>;
};


template<typename Container>
std::string SerializeableContainer(const Container& myContainer) requires SerializeableContainer<Container>
{
	nlohmann::json jsonObject = myContianer;
	return jsonObject.dump(2);
}

// vector 리턴 
template <typename Container>
Container DeserializeContainerFromFile(const std::string& filename)
{
	std::ifstream inputFile(filename);
	nlohmann::json jsonObject;
	inputFile >> jsonObject;
	inputFile.close();
	return jsonObject.get<Container>();
}
