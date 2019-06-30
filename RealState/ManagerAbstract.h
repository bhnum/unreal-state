#pragma once

#include <fstream>
#include <functional>
#include <string>
#include <list>

using std::string;
using std::list;

template <typename T>
class Manager
{
public:
	explicit Manager(const string &filename = "") : filename(filename) {}
	virtual ~Manager() {}

	virtual void load() = 0;
	virtual void save() = 0;
	virtual void bind() {}
	virtual void validate(const T &t) {}
	virtual int generate_uid()
	{
		// generate unique id
		int id;
		do
		{
			id = longrand(1000'0000, 1'0000'0000); // 8 digits
		} while (query(id) != nullptr);
		return id;
	}

	virtual void add(T &t) = 0;
	virtual void edit(const T &t) = 0;
	virtual void remove(int id) = 0;
	virtual T* query(int id) = 0;
	virtual list<T*> query(std::function<bool(T &t)> predicate) = 0;

protected:
	string filename;
private:
	Manager(const Manager&) {}
};


template <typename T>
class ValManager : public Manager<T>
{
public:
	ValManager(const string &filename = "") : Manager<T>(filename) {}

	virtual void load() override
	{
		data.erase(data.begin(), data.end());

		std::ifstream inf(filename);
		if (inf.fail())
			return; // First application run

		while (true)
		{
			T t;
			inf >> t;
			if (inf.eof()) break;
			if (inf.fail())
				throw std::runtime_error("Error reading file \"" + filename + "\"");
			data.push_back(t);
		}
		inf.close();
		bind();
	}

	virtual void save() override
	{
		if (filename == "") return;

		std::ofstream outf(filename, std::ios::trunc);
		if (outf.fail())
			throw std::runtime_error("Error opening file \"" + filename + "\" for writing.");

		for (auto i = data.begin(); i != data.end(); i++)
			outf << *i;
		outf.close();
	}

	virtual list<T> &get()
	{
		return data;
	}

	virtual void add(T &t) override
	{
		validate(t);

		t.set_id(generate_uid());

		data.push_back(t);
		save();
	}

	virtual void edit(const T &t) override
	{
		T *old = query(t.get_id());
		if (old == nullptr)
			throw std::invalid_argument(string(typeid(T).name()) + " id \"" + std::to_string(t.get_id()) + "\" does not exist.");
		if (typeid(*old) != typeid(t))
			throw std::invalid_argument(string(typeid(t).name()) + " is not the same as " + typeid(*old).name());

		validate(t);

		*old = t;
		save();
	}

	virtual void remove(int id) override
	{
		bool found = false;
		for (auto i = data.begin(); i != data.end(); i++)
			if (i->get_id() == id)
			{
				data.erase(i);
				found = true;
				break;
			}
		if (!found)
			throw std::invalid_argument(string(typeid(T).name()) + " id \"" + std::to_string(id) + "\" does not exist.");

		save();
	}

	virtual T *query(int id) override
	{
		for (auto i = data.begin(); i != data.end(); i++)
			if (i->get_id() == id)
				return &*i;
		return nullptr;
	}

	virtual list<T*> query(std::function<bool(T &t)> predicate) override
	{
		list<T*> found;
		for (auto i = this->data.begin(); i != this->data.end(); i++)
			if (predicate(*i))
				found.push_back(&*i);
		return found;
	}

protected:
	list<T> data;
};

template <typename T>
class RefManager : public Manager<T>
{
public:
	RefManager(const string &filename = "") : Manager<T>(filename) {}

	virtual ~RefManager()
	{
		for (auto i = this->data.begin(); i != this->data.end(); i++)
			delete *i;
	}

	virtual void load() override
	{
		for (auto i = this->data.begin(); i != this->data.end(); i++)
			delete *i;
		data.erase(data.begin(), data.end());

		std::ifstream inf(filename);
		if (inf.fail())
			return; // First application run

		while (true)
		{
			T* t;
			inf >> t;
			if (inf.eof()) break;
			if (inf.fail())
				throw std::runtime_error("Error reading file \"" + filename + "\"");
			data.push_back(t);
		}
		inf.close();
		bind();
	}

	void virtual save() override
	{
		if (filename == "") return;

		std::ofstream outf(filename, std::ios::trunc);
		if (outf.fail())
			throw std::runtime_error("Error opening file \"" + filename + "\" for writing.");
		for (auto i = data.begin(); i != data.end(); i++)
		{
			const T *t = *i;
			operator<<(outf, t);
		}

		outf.close();
	}

	virtual list<T*> &get()
	{
		return data;
	}

	virtual void add(T &t) override
	{
		validate(t);

		t.set_id(generate_uid());

		data.push_back(t.clone());
		save();
	}

	virtual void edit(const T &t) override
	{
		T *old = query(t.get_id());
		if (old == nullptr)
			throw std::invalid_argument(string(typeid(T).name()) + " id \"" + std::to_string(t.get_id()) + "\" does not exist.");
		if (typeid(*old) != typeid(t))
			throw std::invalid_argument(string(typeid(t).name()) + " is not the same as " + typeid(*old).name());

		validate(t);

		old->assign(t);
		save();
	}

	virtual void remove(int id) override
	{
		bool found = false;
		for (auto i = data.begin(); i != data.end(); i++)
			if ((*i)->get_id() == id)
			{
				delete *i;
				data.erase(i);
				found = true;
				break;
			}
		if (!found)
			throw std::invalid_argument(string(typeid(T).name()) + " id \"" + std::to_string(id) + "\" does not exist.");

		save();
	}

	virtual T *query(int id) override
	{
		for (auto i = data.begin(); i != data.end(); i++)
			if ((*i)->get_id() == id)
				return *i;
		return nullptr;
	}

	virtual list<T*> query(std::function<bool(T &t)> predicate) override
	{
		list<T*> found;
		for (auto i = this->data.begin(); i != this->data.end(); i++)
			if (predicate(**i))
				found.push_back(*i);
		return found;
	}

protected:
	list<T*> data;
};
