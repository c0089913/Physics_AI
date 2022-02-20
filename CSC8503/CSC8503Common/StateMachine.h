#pragma once
#include <vector>
#include <map>
#include<string>
namespace NCL {
	namespace CSC8503 {

		class State;
		class StateTransition;

		typedef std::multimap<State*, StateTransition*> TransitionContainer;
		typedef TransitionContainer::iterator TransitionIterator;

		class StateMachine	{
		public:
			StateMachine();
			virtual ~StateMachine();

			void AddState(State* s);
			void AddTransition(StateTransition* t);

			virtual void Update(float dt);

			std::string GetActiveStateName(); 
		protected:
			State * activeState;

			std::vector<State*> allStates;

			TransitionContainer allTransitions;
		};
	}
}