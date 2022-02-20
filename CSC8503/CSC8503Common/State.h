#pragma once
#include <string>
#include<functional>
namespace NCL {
	namespace CSC8503 {
		/*class State		{
		public:
			State() {}
			virtual ~State() {}
			virtual void Update() = 0; //Pure virtual base class
		};

		typedef void(*StateFunc)(void*);

		class GenericState : public State		{
		public:
			GenericState(StateFunc someFunc, void* someData) {
				func		= someFunc;
				funcData	= someData;
			}
			virtual void Update() {
				if (funcData != nullptr) {
					func(funcData);
				}
			}
		protected:
			StateFunc func;
			void* funcData;
		};*/
		typedef std::function <void(float)> StateUpdateFunction;

		class State {
		public:
			State() {}
			State(StateUpdateFunction someFunc, std::string n = "") {
				func = someFunc;
				name = n;
			}
			void Update(float dt) {
				if (func != nullptr) {
					func(dt);
				}
			}
			std::string GetName() { return name; }
		protected:
			StateUpdateFunction func;
			std::string name;
		};
	}
}