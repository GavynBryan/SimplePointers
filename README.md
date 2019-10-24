# Quick explanation of how pointers work in C++
Most online resources explaining how pointers work in C/C++ tend to be convoluted or don't demonstrate why they're useful for the programmer. This is actually really confusing considering it's one of the most important features of C++ that separates it from the other languages. 

This guide is made to organically demonstrate how pointers work in a way that you will understand their use and be ready to use them with confidence as soon as possible. No convoluted , unrelatable and unrealistic code examples.

## At its bare minimum

```
#include <iostream>

int main()
{
	int number = 10;
	int* pointer (&number); //the pointer is denoted by the asterisk
	std::cout << *pointer << " resides at " << pointer << "\n";
	number += 20;
	std::cout << "But now, " << pointer << " is " << *pointer << "!" << std::endl;
}
```

The output would look something like:
> 10 resides at 0031FC54
> But now, 0031FC54 is 30!

Notice how we changed the value of \*pointer without touching it! The &ampersand contains the memory addressis called the "reference". The reference is what "feeds" the pointer the information it needs to retain the original information. The pointer *points* to the value of the memory address. Pointers are useful for referencing the original instance of something without making any copies in the memory. 
If you were to strip the pointers from this example, "pointer" would have retained the old value of 10, while "number" increases to 30. I called the constructor for the pointer variable to demonstrate that references are usually passed through arguments. 

But raw pointer usage tends to be a bit outdated. C++ has been updated with features that free us from (most of) the stress of managing memory addresses manually. Let's take a look at smart pointers.

## Smart Pointers

I'll admit, keeping the memory address of integers is probably *not* going to be how most of you are going to be using pointers. Most people want to keep references to instances of classes. If it were 2005, you could handle that with raw pointers and new() declarations, but as of writing this guide, it's 2019 and we've *long* since moved past manually managing memory for class pointers.

Let's include \<memory> in our header.
  
```
#include <iostream>
#include <memory>
```
This allows us to access the smart pointers under the std namespace. I'll also be including the string and vector libraries for the purpose of this example. 

```
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

	people.push_back(std::move(Bob));
	people.push_back(std::move(Jeff));
	people.push_back(std::move(Chad));

	std::cout << "And they all come together and say, \n\"And we all live inside of this vector!\"" << std::endl;

}
  ```
Keep in mind this is *not* a guide on proper class structure. ;) We've defined a class, person, to start off. I'm "person" to keep the example relatable and easy to understand. In our main function, we create 4 unique pointers, each holding a reference to a new person, and add them to the vector of people. We use std::move() to give the burden of the memory address to the vector.

Notice how all 4 of them were added to the vector of people, except Stacy. I did that intentionally; her pointer still resides in the main scope instead of the vector. To find out what this really means, add this block of code below the last std::cout

```
	std::cout << Bob.get() << std::endl;
	std::cout << Stacy.get() << std::endl;
 ```
 The last two lines of the program should be something like 
 
 > 00000000
 
 > 005ED188
 
 Because the items are contained inside the pointers, we use get() to get the values inside the pointers. The first line is Bob's value, as you can see it is null. The second line is Stacy's value, as you can see, it is not null. This is because the Ptrs' memory have been moved inside the vector. In fact, because the entire Ptr was moved, you would get similar results if you removed the get() from these lines. 
 
> 00000000

> 0065D188

So now we know how ownership works with unique pointers. Once you move it to another scope or list, it resides there. Let's put Stacy in with everyone else now that we understand. 

```
	people.push_back(std::move(Bob));
	people.push_back(std::move(Jeff));
	people.push_back(std::move(Chad));
	people.push_back(std::move(Stacy)); //Welcome, Stacy! :)
```

## Combining Smart & Raw Pointers

So we have Ptrs containing people. But, now we also need a reference to a certain person. It's actually a really complicated solution.

```
	Ptr Bob(new person("Bob"));
	Ptr Jeff(new person("Jeff"));
	Ptr Chad(new person("Chad"));
	Ptr Stacy(new person("Stacy"));

	person* pBob = Bob.get();
```
We apply it like so

```
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
```
Notice that -> on pBob->getName(). The arrow *points* to the value being accessed by the pointer. You really can't talk about "pointers" in a neutral way without using the word "point". ;) 

**It's important to understand the difference between the pointer and the value itself.** You cannot say pBob.getName() because pBob actually *does not have a member called getName().* However, the "person" that pBob is pointing to DOES have a getName(), so we use the -> arrow to point to the real Person. 

The output of this program would look like:


> A child is born! They say, "Hey, I'm Bob! :)"

> A child is born! They say, "Hey, I'm Jeff! :)"

> A child is born! They say, "Hey, I'm Chad! :)"

> A child is born! They say, "Hey, I'm Stacy! :)"

> And they all come together and say,
> "And we all live inside of this vector!"

> Bob says, "AND HEY, I'M STILL BOB! :)"

Hopefully after this quick, simple and lazy guide, you understand the differences between pointers, references, and their values. And you also understand their general application in everyday programming. I tried explaining them without using overly technical terms beyond the comprehension of a beginner. If you found this useful, feel free to share it. If you have any criticisms, feel free to make a pull request and I'll review it. 
