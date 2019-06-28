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
	int id;
	int buildarea;
	int numberofrooms;
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
	int baseprice;
	string address;
};

class NothernVilla : public Villa
{
public:
	NothernVilla() {}

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
	int frontyardarea;
	int backyardarea;

};

class SouthernVilla : public Villa
{
public:
	SouthernVilla() {}

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
	int yardarea;
	int garagearea;

};

class ApartmentBuilding
{
public:
	ApartmentBuilding() {}

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
	int id;
	int totalarea;
	int baseprice;
	bool haselevator;
	int numberoffloors;
	int numberofapartments;
	string address;
	string photodata;

};

class Apartment : public Residence
{
public:
	Apartment() {}

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
		throw std::logic_error("Not implemented!");
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

protected:
	int floornumber;
	ApartmentBuilding *building;
};
