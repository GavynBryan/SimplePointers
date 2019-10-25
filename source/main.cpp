#include <iostream>
#include <memory>
#include <string>
#include <vector>

using std::string;

class person {
private:
	string name;
public:
	person(string _name)
		:name(_name)
	{
		std::cout << "A child is born! They say, \"Hey, I'm " << name << "! :)\"" << std::endl;
	}

	string getName() {return name;}
};

int main()
{
	typedef std::unique_ptr<person> Ptr;
	std::vector<Ptr> people;

	Ptr Bob(new person("Bob"));
	Ptr Jeff(new person("Jeff"));
	Ptr Chad(new person("Chad"));
	Ptr Stacy(new person("Stacy"));

	person* pBob = Bob.get();

	people.push_back(std::move(Bob));
	people.push_back(std::move(Jeff));
	people.push_back(std::move(Chad));
	people.push_back(std::move(Stacy)); 

	std::cout << "And they all come together and say, \n\"And we all live inside of this vector!\"" << std::endl;

	std::cout << pBob->getName() << " says, \"AND HEY, I'M STILL BOB! :)\"" << std::endl;
}
