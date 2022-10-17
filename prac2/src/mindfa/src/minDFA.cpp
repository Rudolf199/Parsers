#include "./minDFA.hpp"

#include <iostream>
DFA::DFA(std::vector<Transition> transitions, std::vector<State> states,
         std::string alphabet) {
  all_states_ = states;
  transitions_ = transitions;
  alphabet_ = alphabet;
}
void DFA::PrintDFA() {
  std::cout << "--------------DFA--------------\n";
  std::cout << "\n------------------------STATES------------------\n";
  std::cout << "NAME    INDEX     TRASH      FINAL      START\n";
  for (size_t i = 0; i < all_states_.size(); ++i) {
    std::cout << all_states_[i].name << "         " << all_states_[i].index
              << "         " << all_states_[i].IsTrash << "        "
              << all_states_[i].IsFinal << "         " << all_states_[i].isStart
              << "\n";
  }
  std::cout << "--------------------Transitions----------------------\n";
  std::cout << "START          PATH           END\n";
  for (size_t i = 0; i < transitions_.size(); ++i) {
    std::cout << all_states_[transitions_[i].start].name << "               "
              << transitions_[i].path << "               "
              << all_states_[transitions_[i].end].name << "\n";
  }
}
std::vector<State> StatesInit(int number_of_states) {
  std::string state_type, name;
  std::vector<State> states;
  for (int i = 0; i < number_of_states; ++i) {
    State state = State();
    std::cout << "Name of " << i << " state:\n";
    std::cin >> name;
    state.name = name;
    std::cout << "Is is final state or start or trash state?(type 'final' or "
                 "'start' or 'both or 'trash' or 'none'):\n";
    std::cin >> state_type;
    state.IsFinal = false;
    state.IsTrash = false;
    state.isStart = false;
    if (state_type == "final") {
      state.IsFinal = true;
    }
    if (state_type == "start") {
      state.isStart = true;
    }
    if (state_type == "both") {
      state.IsFinal = true;
      state.isStart = true;
    }
    if (state_type == "trash") {
      state.IsTrash = true;
    }
    if (state_type == "none") {
      state.IsFinal = false;
      state.isStart = false;
      state.IsTrash = false;
    }
    state.index = i;
    states.push_back(state);
  }
  int starts = 0, finals = 0;
  for (size_t i = 0; i < states.size(); ++i) {
    if (states[i].isStart) {
      starts++;
    }
    if (states[i].IsFinal) {
      finals++;
    }
  }
  if (starts != 1 || finals == 0) {
    std::cout << "You entered some cringe!\n";
    abort();
  }
  return states;
}
std::vector<Transition> TransitionsInit(int num_of_transitions,
                                        std::vector<State>& states,
                                        std::string alphabet) {
  std::vector<Transition> transitions;
  for (int i = 0; i < num_of_transitions; ++i) {
    Transition transition = Transition();
    std::cout << "Enter start and end indexes for transition:\n";
    int start, end;
    std::cin >> start >> end;
    transition.start = start;
    transition.end = end;
    char path;
    std::cout << "Enter path for " << i << "-th transition\n";
    std::cin >> path;
    if (path == '$') {
      std::cout << "What a cringe...\n";
      abort();
    }
    transition.path = path;
    states[start].path_counter++;
    states[start].paths += path;
    states[start].friends += states[end].name;
    std::sort(states[start].paths.begin(), states[start].paths.end());
    if (states[start].path_counter > (int)alphabet.size()) {
      std::cout << "We are doing DFA here......\n";
      abort();
    }
    transitions.push_back(transition);
  }
  if (transitions.size() != states.size() * alphabet.size()) {
    std::cout << "Not a FDFA\n";
    abort();
  }
  return transitions;
}

void DFA::Inversion() {
  for (size_t i = 0; i < all_states_.size(); ++i) {
    if (all_states_[i].IsFinal) {
      all_states_[i].IsFinal = false;
    } else if (!all_states_[i].IsFinal) {
      all_states_[i].IsFinal = true;
    }
  }
}
bool Intersection(std::string str1, std::string str2) {
  for (size_t i = 0; i < str1.size(); ++i) {
    for (size_t j = 0; j < str2.size(); ++j) {
      if (str1[i] == str2[j]) {
        return true;
      }
    }
  }
  return false;
}
bool IsInVector(int index, std::vector<int> vect) {
  for (size_t i = 0; i < vect.size(); ++i) {
    if (vect[i] == index) {
      return true;
    }
  }
  return false;
}
Groups DFA::MinDFAgroups() {
  int movements = 0;
  Groups classes = Groups();
  Group finals = Group();
  Group normals = Group();
  normals.index = 0;
  finals.index = 1;
  for (size_t i = 0; i < all_states_.size(); ++i) {
    if (all_states_[i].IsFinal) {
      finals.states.push_back(all_states_[i]);
      finals.names += all_states_[i].name;
      finals.indexes.push_back(all_states_[i].index);
    } else {
      normals.states.push_back(all_states_[i]);
      normals.names += all_states_[i].name;
      normals.indexes.push_back(all_states_[i].index);
    }
  }
  classes.groups.push_back(normals);
  classes.groups.push_back(finals);
  int inaction = 0;
  size_t size = 2;
  for (size_t i = 0; i < alphabet_.size(); ++i) {
    Partition(alphabet_[i], classes, movements, size);
    if (movements == 0) {
      inaction++;
    }
    if (movements > 0) {
      inaction = 0;
    }
    if (inaction == alphabet_.size()) {
      break;
    }
    movements = 0;
  }
  return classes;
}
void DFA::NewGroups(Group& kickout, Groups& classes) {
  for (size_t i = 0; i < kickout.states.size(); ++i) {
    // iterating through kickout
    Group temp = Group();
    // creating a new group for i-th element
    temp.states.push_back(kickout.states[i]);
    temp.index = classes.groups.size();
    temp.names += kickout.states[i].name;
    temp.indexes.push_back(kickout.states[i].index);
    // pushing him there
    for (size_t j = i + 1; j < kickout.states.size(); j++) {
      // iterating through other states to see if someone goes w him
      if (kickout.states[j].current == kickout.states[i].current) {
        // if there is a friend for him, we push him too
        temp.states.push_back(kickout.states[j]);
        temp.index = classes.groups.size();
        temp.names += kickout.states[j].name;
        temp.indexes.push_back(kickout.states[j].index);
        kickout.states.erase(kickout.states.begin() + j);
        // and we delete him from kickout to not check him again
        j = j - 1;
        // we step back by one cuz we deleted an element
      }
    }
    // after we collected all friends of i-th state, we delete him
    kickout.states.erase(kickout.states.begin() + i);
    // i = -1;
    // step back to start, because of delete
    classes.groups.push_back(temp);
    if (kickout.states.size() == 1) {
      Group last = Group();
      last.index = classes.groups.size();
      last.indexes.push_back(kickout.states[0].index);
      last.names = kickout.states[0].name;
      last.states.push_back(kickout.states[0]);
      classes.groups.push_back(last);
      break;
    }
  }
}
bool DFA::AllInOne(Group group) {
  int sum = 0;
  if (group.states.size() == 1) {
    return true;
  }
  for (size_t k = 0; k < alphabet_.size(); k++) {
    for (size_t j = 1; j < group.states.size(); ++j) {
      if (WhereItGoes(group.states[0].index, alphabet_[k]) ==
          WhereItGoes(group.states[j].index, alphabet_[k])) {
        sum++;
      }
    }
  }
  if (sum == (group.states.size() - 1) * alphabet_.size()) {
    return true;
  }
  return false;
}
void DFA::Partition(char path, Groups& classes, int& movements, size_t& size) {
  for (size_t i = 0; i < size; ++i) {
    // iterating through all groups in class to divide them based on path
    if (classes.groups[i].states.size() == 1) {
      continue;
    }
    if (AllInOne(classes.groups[i])) {
      continue;
    }
    Group kickout = Group();
    // collecting the kicked states in new group caled kickout
    for (size_t j = 0; j < classes.groups[i].states.size(); ++j) {
      // iterating through all states of -th group in classes
      if (!IsInVector(WhereItGoes(classes.groups[i].states[j].index, path),
                      classes.groups[i].indexes)) {
        // if j-th state of i-th group in classes goes somewhere to other group,
        // then we kick him out
        classes.groups[i].states[j].current = FindGroupIndex(
            WhereItGoes(classes.groups[i].states[j].index, path), classes);
        // finding the index of group where it goes
        kickout.states.push_back(classes.groups[i].states[j]);
        // push him to the kickout group
        DeleteIndex(classes.groups[i].indexes,
                    classes.groups[i].states[j].index);
        ClearName(classes.groups[i].names, classes.groups[i].states[j].name);
        classes.groups[i].states.erase(classes.groups[i].states.begin() + j);
        // completely erase him from i-th group

        // remember that there was a movement in i-th group
        movements++;
        j = j - 1;
      }
    }
    // (MAIN IDEA) collecting new groups and sending them to Groups and
    // returning now after iterating throught i-th group, we add new groups that
    // we made to classes
    NewGroups(kickout, classes);
    printclasses(classes);
  }
  size = classes.groups.size();
}
void DFA::ClearName(std::string& names, std::string name) {
  int start_position_to_erase = names.find(name);
  names.erase(start_position_to_erase, name.size());
}
void DeleteIndex(std::vector<int>& vect, int index) {
  for (size_t i = 0; i < vect.size(); ++i) {
    if (vect[i] == index) {
      vect.erase(vect.begin() + i);
    }
  }
}
int DFA::FindGroupIndex(int index, Groups& classes) {
  for (size_t i = 0; i < classes.groups.size(); ++i) {
    for (size_t k = 0; k < classes.groups[i].indexes.size(); ++k) {
      if (index == classes.groups[i].indexes[k]) {
        return classes.groups[i].index;
      }
    }
  }
  return 0;
}
int DFA::WhereItGoes(int index, char path) {
  for (size_t i = 0; i < transitions_.size(); ++i) {
    if (transitions_[i].start == index && transitions_[i].path == path) {
      return transitions_[i].end;
    }
  }
  return 0;
}
void printclasses(Groups classes) {
  std::cout << "CLASSES\n";
  for (size_t i = 0; i < classes.groups.size(); ++i) {
    std::cout << "|";

    std::cout << classes.groups[i].names << " ";
  }
  std::cout << "\nCLASSES\n";
}
bool ContainsFinal(Group group) {
  for (size_t i = 0; i < group.states.size(); ++i) {
    if (group.states[i].IsFinal) {
      return true;
    }
  }
  return false;
}
bool ContainsStart(Group group) {
  for (size_t i = 0; i < group.states.size(); ++i) {
    if (group.states[i].isStart) {
      return true;
    }
  }
  return false;
}
DFA DFA::MinDFA(Groups classes) {
  std::vector<State> states;
  std::vector<Transition> transitions;
  for (size_t i = 0; i < classes.groups.size(); ++i) {
    State state = State();
    state.index = i;
    state.name = classes.groups[i].names;
    if (ContainsFinal(classes.groups[i])) {
      state.IsFinal = true;
    }
    if (ContainsStart(classes.groups[i])) {
      state.isStart = true;
    }
    states.push_back(state);
    for (size_t j = 0; j < alphabet_.size(); ++j) {
      Transition transition = Transition();
      transition.path = alphabet_[j];
      transition.start = states[i].index;
      transition.end = FindGroupIndex(
          WhereItGoes(classes.groups[i].states[0].index, alphabet_[j]),
          classes);
      transitions.push_back(transition);
    }
  }
  DFA dfa(transitions, states, alphabet_);
  return dfa;
}