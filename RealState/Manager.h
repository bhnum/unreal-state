#pragma once

#include "User.h"
#include "Residence.h"
#include "Contract.h"
#include "ManagerAbstract.h"

class UserManager : ValManager<User>
{
public:
	explicit UserManager(const string &filename = "users.txt");

 	using ValManager<User>::load;
	using ValManager<User>::save;

	virtual void validate(const User &user) override;

	list<User> &get_users() { return get(); }
	void register_user(User &user) { add(user); }
	void edit_user(const User &user) { edit(user); }
	void delete_user(int id) { remove(id); }

	bool authenticate_user(const string &username, const string &password);
	User *query_user(int id) { return query(id); };
	User *query_user(const string &username);
	list<User*> query_user(std::function<bool(User&)> predicate) { return query(predicate); }

	void update_logintime(int id);
	void update_logouttime(int id);
};


class ResidenceManager : RefManager<Residence>
{
public:
	explicit ResidenceManager(const string &filename = "residences.txt");

	using RefManager<Residence>::load;
	using RefManager<Residence>::save;

	list<Residence*> &get_residences() { return RefManager<Residence>::get(); }
	void add_residence(Residence& residence) { RefManager<Residence>::add(residence); }
	void edit_residence(const Residence& residence) { RefManager<Residence>::edit(residence); }
	void delete_residence(int id) { RefManager<Residence>::remove(id); };
	Residence* query_residence(int id) { return RefManager<Residence>::query(id); }
	list<Residence*> query_residence(std::function<bool(Residence&)> predicate)
	{
		return RefManager<Residence>::query(predicate); 
	}
private:
	virtual void bind() override;
};

class ContractManager : RefManager<Contract>
{
public:
	explicit ContractManager(ResidenceManager &resman, UserManager &userman,
		const string &filename = "contracts.txt");

	using RefManager<Contract>::load;
	using RefManager<Contract>::save;

	virtual void load_data(std::ifstream &inf) override;
	virtual void save_data(std::ofstream &outf) override;

	int get_commissionrate() const { return commissionrate; }
	void set_commissionrate(int _commissionrate) { commissionrate = _commissionrate; save(); }

	ResidenceManager& get_residenceManager() const { return resman; }
	UserManager& get_userManager() const { return userman; }

	list<Contract*> &get_contracts() { return RefManager<Contract>::get(); }
	void add_contract(Contract& contract) { RefManager<Contract>::add(contract); }
	void edit_contract(const Contract& contract) { RefManager<Contract>::edit(contract); }
	void delete_contract(int id) { RefManager<Contract>::remove(id); };
	Contract* query_contract(int id) { return RefManager<Contract>::query(id); }
	list<Contract*> query_contract(std::function<bool(Contract&)> predicate)
	{
		return RefManager<Contract>::query(predicate);
	}

	bool is_residence_taken(int id, bool be_verified = true);

private:
	virtual void bind() override;
	int commissionrate = 5;
	ResidenceManager &resman;
	UserManager &userman;
};

