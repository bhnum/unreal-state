#pragma once
#include "Residence.h"

enum class ContractType
{
	Rent, Sale
};

class Contract
{
public:
	Contract() {}

	virtual ContractType get_type() const = 0;
	virtual int get_finalprice() const = 0;

	virtual std::ostream &output(std::ostream &out) const
	{
		out << id << ',' << commissionrate << ',';
	}

	virtual std::istream &input(std::istream &in)
	{
		in >> id;
		in.ignore();
		in >> commissionrate;
		in.ignore();
	}

	int get_id() const { return id; }
	void set_id(int _id) { id = _id; }

	Residence *get_residence() const { return residence; }
	void set_residence(Residence *_residence) { residence = _residence; }

	int get_commissionrate() const { return commissionrate; }
	void set_commissionrate(int _commissionrate) { commissionrate = _commissionrate; }

	friend std::ostream &operator<<(std::ostream &out, const Contract *&r);
	friend std::istream &operator>>(std::istream &in, Contract *&r);

protected:
	int id;
	Residence *residence;
	int commissionrate;

};

class RentConstract : public Contract
{
public:
	RentConstract() {}

	virtual std::ostream &output(std::ostream &out) const
	{
		Contract::output(out);
		out << rentduration << ',';
	}

	virtual std::istream &input(std::istream &in)
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
	int rentduration;

};

class SaleContract : public Contract
{
public:
	SaleContract() {}

	virtual std::ostream &output(std::ostream &out) const
	{
		Contract::output(out);
		out << terms << ',';
	}

	virtual std::istream &input(std::istream &in)
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

