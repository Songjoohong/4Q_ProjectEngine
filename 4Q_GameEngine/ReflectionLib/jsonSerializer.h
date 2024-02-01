#pragma once
#include <string>
#include "nlohmann/json.hpp"

// nlohmann::json ��ü�� ����� ǥ������ nlohmann::json(container)�� nlohmann::json���� ��ȯ�� �����ؾ���
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

// ���� �ϰ� ������ GameObject vector�� �Ѱ��� �� 
// �� ������Ʈ���� ������ �ִ� Component Class�鵵 ����ȭ�� �ǰ�
// �� Component���� ������ �ִ� ��� ���� ���鵵 ����ȭ�� �ѹ��� �Ҹ��� �ǰ��ϰ���� 

// GameObject vector�� �ѱ�� �Ǹ� �����͸� serialize�ؾߵǱ� ������ ����
// Entity ���� serialize�ϰ� Component�� ���� serialize�� �� 
// �ʱ�ȭ ���� �� Entity�� ID���� ���� Component�־��ִ� ���
