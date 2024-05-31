#include <iostream>
#include <string>
#include <vector>
using namespace std;

__interface IReaction {
	virtual void PushReaction() = 0;
};

class Like : public IReaction {
public:
	virtual void PushReaction() override {
		cout << "Subscriber likes\n";
	}
};

class Comment : public IReaction {
	string comment;
public:

	Comment(string m) : comment(m){}

	virtual void PushReaction() override {
		cout << "Comment\t" << comment << "\n";
	}
};

__interface IReel {
	virtual void Create() = 0;
	virtual void AddReaction(IReaction* r) = 0;
};

class Reels : public IReel {
private:
	vector<IReaction*> reactions;
public:
	virtual void Create() override {
		cout << "Create instagram reels\n";
	}
	virtual void AddReaction(IReaction* r) override {
		r->PushReaction();
		reactions.push_back(r);
	}
};

__interface IObserver {
	virtual void Update(bool p) = 0;
	virtual void ReactToReel(IReel* reel, IReaction* r) = 0;
};

class IObservable {
public:
	virtual void AddObserver(IObserver* o) = 0;
	virtual void RemoveObserve(IObserver* o) = 0;
	virtual void Notify() = 0;
	virtual ~IObservable() {}
};

class User : public IObservable {
private:
	vector <IObserver*> follower;
	vector <IReel*> reel;
	bool create_reels;
public:
	/*User(bool p) : create_reels(p) {}*/

	void CreateReels(IReel* r) {
		r->Create();
		create_reels = true;
		Notify();
	}

	void DeleteReels(IReel* r) {
		reel.erase(remove(reel.begin(), reel.end(), r), reel.end());
		create_reels = false;
		Notify();
	}

	void AddObserver(IObserver* o) override {
		follower.push_back(o);
	}

	void RemoveObserve(IObserver* o) override {
		follower.erase(remove(follower.begin(), follower.end(), o), follower.end());
	}

	void Notify() override {
		cout << "\n\nNotify: \n";
		for (auto o : follower) {
			o->Update(create_reels);
		}
	}
};

class Follower : public IObserver {
private:
	IObservable* user;
public:
	Follower(IObservable* obj) : user(obj) {
		obj->AddObserver(this);
	}
	void Update(bool create_reel) override {
		if (create_reel == true)
			cout << "User create reel\n";
		if (create_reel == false)
			cout << "User delete reel\n";
	}
	void ReactToReel(IReel* reel, IReaction* r) override {
		reel->AddReaction(r);
	}
};

int main() {
	User* user = new User;
	IReel* reels = new Reels;
	
	Follower* follower = new Follower(user);
	user->CreateReels(reels);
	follower->ReactToReel(reels,new Like);
	follower->ReactToReel(reels, new Comment("wow!!!"));
	user->DeleteReels(reels);
}