#pragma once
#include <string>
#include "nlohmann/json.hpp"

// nlohmann::json ��ü�� ����� ǥ������ nlohmann::json(container)�� nlohmann::json���� ��ȯ�� �����ؾ���
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

// vector ���� 
template <typename Container>
Container DeserializeContainerFromFile(const std::string& filename)
{
	std::ifstream inputFile(filename);
	nlohmann::json jsonObject;
	inputFile >> jsonObject;
	inputFile.close();
	return jsonObject.get<Container>();
}
