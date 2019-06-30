#pragma once

#include "Residence.h"
#include "User.h"

enum class ContractType
{
	Rent, Sale
};

class Contract
{
public:
	Contract() {}

	virtual Contract &assign(const Contract &r) = 0;
	virtual Contract *clone() = 0;

	virtual ContractType get_type() const = 0;
	virtual int get_finalprice() const = 0;

	virtual void output(std::ostream &out) const
	{
		out << id << ',' << residenceid << ',' << holderid << ',' << commissionrate << ',';
	}

	virtual void input(std::istream &in)
	{
		in >> id;
		in.ignore();
		in >> residenceid;
		in.ignore();
		in >> holderid;
		in.ignore();
		in >> commissionrate;
		in.ignore();
	}

	int get_id() const { return id; }
	void set_id(int _id) { id = _id; }

	int get_residenceid() const { return residenceid; }
	void set_residenceid(int _residenceid) { residenceid = _residenceid; }

	Residence *get_residence() const { return residence; }
	void set_residence(Residence *_residence) { residence = _residence; }

	int get_holderid() const { return holderid; }
	void set_holderid(int _holderid) { holderid = _holderid; }

	User *get_holder() const { return holder; }
	void set_holder(User *_holder) { holder = _holder; }

	int get_commissionrate() const { return commissionrate; }
	void set_commissionrate(int _commissionrate) { commissionrate = _commissionrate; }

	friend std::ostream &operator<<(std::ostream &out, const Contract *&r);
	friend std::istream &operator>>(std::istream &in, Contract *&r);

protected:
	int id = 0;
	int residenceid = 0;
	int holderid = 0;
	Residence *residence;
	User *holder;
	int commissionrate = 0;

};

class RentConstract : public Contract
{
public:
	RentConstract() {}

	virtual Contract &assign(const Contract &r)
	{
		const RentConstract &a = dynamic_cast<const RentConstract&>(r);
		return *this = a;
	}

	virtual Contract *clone()
	{
		return new RentConstract(*this);
	}

	virtual void output(std::ostream &out) const
	{
		Contract::output(out);
		out << rentduration << ',';
	}

	virtual void input(std::istream &in)
	{
		Contract::input(in);
		in >> rentduration;
		in.ignore();
	}

	virtual ContractType get_type() const override
	{
		return ContractType::Rent;
	}

	virtual int get_finalprice() const override
	{
		return get_rent() * rentduration + get_mortgage() + commissionrate * get_mortgage() / 100;
	}

	int get_mortgage() const
	{
		return residence->get_finalprice() / 20;
	}

	int get_rent() const
	{
		return residence->get_finalprice() / 100;
	}

	int get_rentduration() const { return rentduration; }
	void set_rentduration(int _rentduration) { rentduration = _rentduration; }

private:
	int rentduration = 0;

};

class SaleContract : public Contract
{
public:
	SaleContract() {}

	virtual Contract &assign(const Contract &r)
	{
		const SaleContract &a = dynamic_cast<const SaleContract&>(r);
		return *this = a;
	}

	virtual Contract *clone()
	{
		return new SaleContract(*this);
	}

	virtual void output(std::ostream &out) const
	{
		Contract::output(out);
		out << terms << ',';
	}

	virtual void input(std::istream &in)
	{
		Contract::input(in);
		getline(in, terms, ',');
	}

	virtual ContractType get_type() const override
	{
		return ContractType::Sale;
	}

	virtual int get_finalprice() const override
	{
		return (100 + commissionrate) * residence->get_finalprice() / 100;
	}

	string get_terms() const { return terms; }
	void set_terms(string _terms) { terms = _terms; }

private:
	string terms;

};

