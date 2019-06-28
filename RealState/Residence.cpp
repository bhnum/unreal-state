#include "Residence.h"


std::ostream &operator<<(std::ostream &out, const Residence *&r)
{
	out << static_cast<int>(r->get_type()) << ',';
	r->output(out);
	return out;
}

std::istream &operator>>(std::istream &in, Residence *&r)
{
	ResidenceType type;
	in >> reinterpret_cast<int&>(type);
	in.ignore();
	switch (type)
	{
	case ResidenceType::NorthernVilla:
		r = new NothernVilla();
		break;
	case ResidenceType::SouthernVilla:
		r = new SouthernVilla();
		break;
	case ResidenceType::Apartment:
		r = new Apartment();
		break;
	default:
		throw std::logic_error("");
	}
	r->input(in);
	return in;
}

std::ostream &operator<<(std::ostream &out, const ApartmentBuilding &r)
{
	out << r.id << ',' << r.totalarea << ',' << r.baseprice << ',' << r.haselevator << ','
		<< r.numberoffloors << ',' << r.numberofapartments << ',' << r.address << ','
		<< r.photodata << ',';
	return out;
}

std::istream &operator>>(std::istream &in, ApartmentBuilding &r)
{
	in >> r.id;
	in.ignore();
	in >> r.totalarea;
	in.ignore();
	in >> r.baseprice;
	in.ignore();
	in >> r.haselevator;
	in.ignore();
	in >> r.numberoffloors;
	in.ignore();
	in >> r.numberofapartments;
	in.ignore();
	getline(in, r.address, ',');
	getline(in, r.photodata, ',');
	return in;
}
