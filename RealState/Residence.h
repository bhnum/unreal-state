#pragma once

#include <iostream>
#include <string>

using std::string;

enum class ResidenceType
{
	NorthernVilla,
	SouthernVilla,
	Apartment,
};

class Residence
{
public:
	Residence() {}

	virtual ~Residence() {}

	virtual Residence &assign(const Residence &r) = 0;
	virtual Residence *clone() = 0;

	virtual ResidenceType get_type() const = 0;
	virtual int get_finalprice() const = 0;
	virtual int get_baseprice() const = 0;
	virtual int get_totalarea() const = 0;
	virtual string get_address() const = 0;

	virtual void output(std::ostream &out) const
	{
		out << id << ',' << buildarea << ',' << numberofrooms << ',' << photodata << ',';
	}

	virtual void input(std::istream &in)
	{
		in >> id;
		in.ignore();
		in >> buildarea;
		in.ignore();
		in >> numberofrooms;
		in.ignore();
		getline(in, photodata, ',');
	}

	int get_id() const { return id; }
	void set_id(int _id) { id = _id; }

	int get_buildarea() const { return buildarea; }
	void set_buildarea(int _buildarea) { buildarea = _buildarea; }

	int get_numberofrooms() const { return numberofrooms; }
	void set_numberofrooms(int _numberofrooms) { numberofrooms = _numberofrooms; }

	string get_photodata() const { return photodata; }
	void set_photodata(string _photodata) { photodata = _photodata; }

	friend std::ostream &operator<<(std::ostream &out, const Residence *&r);
	friend std::istream &operator>>(std::istream &in, Residence *&r);

protected:
	int id = 0;
	int buildarea = 0;
	int numberofrooms = 0;
	string photodata;
};

class Villa : public Residence
{
public:
	Villa() {}

	virtual void output(std::ostream &out) const override
	{
		Residence::output(out);
		out << baseprice << ',' << address << ',';
	}

	virtual void input(std::istream &in)
	{
		Residence::input(in);
		in >> baseprice;
		in.ignore();
		getline(in, address, ',');
	}

	int get_baseprice() const { return baseprice; }
	void set_baseprice(int _baseprice) { baseprice = _baseprice; }

	virtual string get_address() const override { return address; }
	void set_address(string _address) { address = _address; }

protected:
	int baseprice = 0;
	string address;
};

class NothernVilla : public Villa
{
public:
	NothernVilla() {}

	virtual Residence &assign(const Residence &r) override
	{
		const NothernVilla &a = dynamic_cast<const NothernVilla&>(r);
		return *this = a;
	}

	virtual Residence *clone() override
	{
		return new NothernVilla(*this);
	}

	virtual void output(std::ostream &out) const override
	{
		Villa::output(out);
		out << frontyardarea << ',' << backyardarea << ',';
	}

	virtual void input(std::istream &in)
	{
		Villa::input(in);
		in >> frontyardarea;
		in.ignore();
		in >> backyardarea;
		in.ignore();
	}

	int get_frontyardarea() const { return frontyardarea; }
	void set_frontyardarea(int _frontyardarea) { frontyardarea = _frontyardarea; }

	int get_backyardarea() const { return backyardarea; }
	void set_backyardarea(int _backyardarea) { backyardarea = _backyardarea; }

	virtual ResidenceType get_type() const override
	{
		return ResidenceType::NorthernVilla;
	}

	virtual int get_finalprice() const override
	{
		return baseprice * ((long long)buildarea * 100 + frontyardarea * 30 + backyardarea * 15) / 100;
	}

	virtual int get_totalarea() const override
	{
		return buildarea + frontyardarea + backyardarea;
	}

protected:
	int frontyardarea = 0;
	int backyardarea = 0;

};

class SouthernVilla : public Villa
{
public:
	SouthernVilla() {}

	virtual Residence &assign(const Residence &r) override
	{
		const SouthernVilla &a = dynamic_cast<const SouthernVilla&>(r);
		return *this = a;
	}

	virtual Residence *clone() override
	{
		return new SouthernVilla(*this);
	}

	virtual void output(std::ostream &out) const override
	{
		Villa::output(out);
		out << yardarea << ',' << garagearea << ',';
	}

	virtual void input(std::istream &in)
	{
		Villa::input(in);
		in >> yardarea;
		in.ignore();
		in >> garagearea;
		in.ignore();
	}

	int get_yardarea() const { return yardarea; }
	void set_yardarea(int _yardarea) { yardarea = _yardarea; }

	int get_garagearea() const { return garagearea; }
	void set_garagearea(int _garagearea) { garagearea = _garagearea; }

	virtual ResidenceType get_type() const override
	{
		return ResidenceType::SouthernVilla;
	}

	virtual int get_finalprice() const override
	{
		return baseprice * ((long long)buildarea * 100 + yardarea * 30 + garagearea * 15) / 100;
	}

	virtual int get_totalarea() const override
	{
		return buildarea + yardarea + garagearea;
	}

protected:
	int yardarea = 0;
	int garagearea = 0;

};

class ApartmentBuilding
{
public:
	ApartmentBuilding() {}

	int get_finalprice()
	{
		return totalarea * baseprice * 4 / 5;
	}

	int get_id() const { return id; }
	void set_id(int _id) { id = _id; }

	int get_totalarea() const { return totalarea; }
	void set_totalarea(int _totalarea) { totalarea = _totalarea; }

	int get_baseprice() const { return baseprice; }
	void set_baseprice(int _baseprice) { baseprice = _baseprice; }

	bool get_haselevator() const { return haselevator; }
	void set_haselevator(bool _haselevator) { haselevator = _haselevator; }

	int get_numberoffloors() const { return numberoffloors; }
	void set_numberoffloors(int _numberoffloors) { numberoffloors = _numberoffloors; }

	int get_numberofapartments() const { return numberofapartments; }
	void set_numberofapartments(int _numberofapartments) { numberofapartments = _numberofapartments; }

	string get_address() const { return address; }
	void set_address(string _address) { address = _address; }

	friend std::ostream &operator<<(std::ostream &out, const ApartmentBuilding &r);
	friend std::istream &operator>>(std::istream &in, ApartmentBuilding &r);

protected:
	int id = 0;
	int totalarea = 0;
	int baseprice = 0;
	bool haselevator = false;
	int numberoffloors = 0;
	int numberofapartments = 0;
	string address;
	string photodata;

};

enum class ApartmentUsage
{
	Regular, Business, Administrative,
};

class Apartment : public Residence
{
public:
	Apartment() {}

	virtual Residence &assign(const Residence &r) override
	{
		const Apartment &a = dynamic_cast<const Apartment&>(r);
		return *this = a;
	}

	virtual Residence *clone() override
	{
		return new Apartment(*this);
	}

	virtual void output(std::ostream &out) const override
	{
		Residence::output(out);
		out << floornumber << ',' << building->get_id() << ',';
	}

	virtual void input(std::istream &in)
	{
		Residence::input(in);
		in >> floornumber;
		in.ignore();
	}

	int get_floornumber() const { return floornumber; }
	void set_floornumber(int _floornumber) { floornumber = _floornumber; }

	ApartmentBuilding *get_building() const { return building; }
	void set_building(ApartmentBuilding *_building) { building = _building; }

	virtual ResidenceType get_type() const override
	{
		return ResidenceType::Apartment;
	}

	virtual int get_finalprice() const override
	{
		int initial_price = buildarea * get_baseprice();
		int price = initial_price;
		price += initial_price * (floornumber - 1) * 5 / 100;
		price += initial_price * (numberofrooms - 1) * 8 / 100;
		if (building->get_haselevator())
			price += initial_price / 50;
		switch (usage)
		{
		case ApartmentUsage::Regular:
			break;
		case ApartmentUsage::Business:
			price += initial_price / 2;
			break;
		case ApartmentUsage::Administrative:
			price += initial_price * 3 / 10;
			break;
		default:
			throw std::logic_error("");
			break;
		}
		return price;
	}

	virtual int get_totalarea() const override
	{
		return building->get_totalarea();
	}

	virtual int get_baseprice() const override
	{
		return building->get_baseprice();
	}

	virtual string get_address() const override
	{
		return building->get_address();
	}

	ApartmentUsage get_usage() const { return usage; }
	void set_usage(ApartmentUsage _usage) { usage = _usage; }

protected:
	int floornumber = 0;
	ApartmentUsage usage;
	ApartmentBuilding *building;
};
