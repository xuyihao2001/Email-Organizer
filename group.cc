#include "group.h"

EmailNode::EmailNode(const std::string &email): email{email} {}

EmailNode::~EmailNode() {}

GroupNode::GroupNode(Group *group): group{group} {};

GroupNode::~GroupNode() {
	delete group;
}

Group::Group(const std::string &name): groupName{name} {}

Group::~Group() {
	EmailNode *curnode = en;
	EmailNode *prevnode = nullptr;
	while (curnode) {
		prevnode = curnode;
		curnode = curnode->next;
		delete prevnode;
	}
	en = nullptr;

	GroupNode *gn = subgroups;
	GroupNode *prevgn = nullptr;
	while (gn) {
		prevgn = gn;
		gn = gn->next;
		delete prevgn;
	}
	subgroups = nullptr;
}

std::string Group::name() const {
	return groupName;
}

std::string Group::emails() const {
	std::string s = "[";
	EmailNode *curnode = en;
	while (curnode) {
		if (curnode->prev) {
			s += ", ";
		}
		s = s + curnode->email;
		curnode = curnode->next;
	}
	s += "]";
	return s;
}

void Group::addGroup(Group *group) {
	GroupNode *temp = new GroupNode{group};

	GroupNode *curnode = subgroups;
	GroupNode *prevnode = nullptr;

	while (curnode) {
		if (group->groupName < (curnode->group)->groupName) {
			temp->prev = curnode->prev;
			temp->next = curnode;
			if (curnode->prev) {
				curnode->prev->next = temp;
			} else {
				subgroups = temp;
			}
			curnode->prev = temp;
			return;
		}
		prevnode = curnode;
		curnode = curnode->next;
	}
	temp->prev = prevnode;
	if (prevnode) {
		prevnode->next = temp;
	} else {
		subgroups = temp;
	}
}

void Group::addAddress(const std::string &name) {
	EmailNode *temp = new EmailNode{name};
	EmailNode *curnode = en;
	EmailNode *prevnode = nullptr;

	while (curnode) {
                if (name < curnode->email) {
                        temp->prev = curnode->prev;
                        temp->next = curnode;
                        if (curnode->prev) {
                                curnode->prev->next = temp;
                        } else {
                                en = temp;
                        }
                        curnode->prev = temp;
                        return;
                }
                prevnode = curnode;
                curnode = curnode->next;
        }
        temp->prev = prevnode;
        if (prevnode) {
                prevnode->next = temp;
        } else {
                en = temp;
        }
}

bool GroupNode::removeNode(const std::string &name) {
	if (name == group->groupName) {
		prev->next = next;
		if (next) {
                	next->prev = prev;
		}
                delete this;
                return true;
	} else if (group->removeGroup(name)) {
		return true;
	} else {
		if (next) {
			return next->removeNode(name);
		} else {
			return false;
		}
	}
}

bool Group::removeGroup(const std::string &name) {
	if (subgroups == nullptr) {
                return false;
        } else if (name == subgroups->group->groupName) {
                GroupNode *temp = subgroups;
                subgroups = subgroups->next;
		delete temp;
                if (subgroups) {
			subgroups->prev = nullptr;
		}
                return true;
	} else {
		return subgroups->removeNode(name);
	}
}

bool Group::removeAddress(const std::string &name) {
	EmailNode *curnode = en;
	while (curnode) {
		if (name == curnode->email) {
			if (curnode->prev) {
				curnode->prev->next = curnode->next;
			} else {
				en = curnode->next;
			}
			if (curnode->next) {
				curnode->next->prev = curnode->prev;
			}
			delete curnode;
			return true;
		}
		curnode = curnode->next;
	}
	if (subgroups) {
		return subgroups->removeAddNode(name);
	}
	return false;
}

bool GroupNode::removeAddNode(const std::string &name) {
	bool temp = group->removeAddress(name);
	if (temp) {
		return temp;
	} else if (next) {
		return next->removeAddNode(name);
	} else {
		return false;
	}
}

GroupNode *Group::findGroup(const std::string &name) {
	if (subgroups == nullptr) {
		return nullptr;
	} else {
		return subgroups->findNode(name);
	}
}

GroupNode *GroupNode::findNode(const std::string &name) {
	if (name == group->groupName) {
		return this;
	} else {
		GroupNode *temp = group->findGroup(name);
		if (temp) {
			return temp; 
		} else if (next) {
			return next->findNode(name);
		} else {
			return nullptr;
		}
	}
}

EmailNode *Group::findAddress(const std::string &name) {
	EmailNode *curnode = en;
        while (curnode) {
		if (name == curnode->email) {
			return curnode;
		}
		curnode = curnode->next;
	}
	if (subgroups) {
		return subgroups->findAddNode(name);
	} else {
		return nullptr;
	}
}

EmailNode *GroupNode::findAddNode(const std::string &name) {
	EmailNode *temp = group->findAddress(name);
	if (temp) {
		return temp;
	} else if (next) {
		return next->findAddNode(name);
	} else {
		return nullptr;
	}
}

std::ostream &operator<<(std::ostream &out, GroupNode &node) {
	return out << *(node.group);
}

std::ostream &Group::addTab(std::ostream &out, int tabAmount) {
	int tab = tabAmount;
        while (tab) {
                out << '\t';
                --tab;
        }
        out << name() << std::endl;

        tab = tabAmount;
        while (tab) {
                out << '\t';
                --tab;
        }
        out << emails() << std::endl;

        if (subgroups == nullptr) {
                return out;
        }
        return subgroups->tabNode(out, tabAmount+1);
}

std::ostream &GroupNode::tabNode(std::ostream &out, int tabAmount) {
	if (next) {
		return next->tabNode(group->addTab(out, tabAmount), tabAmount);
        } else {
        	return group->addTab(out, tabAmount);
        }
}

std::ostream &operator<<(std::ostream &out, Group &group) {
	return group.addTab(out, 0);
}

std::ostream &operator<<(std::ostream &out, EmailNode &en) {
	return out << en.email;
}
