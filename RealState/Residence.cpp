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
	case ResidenceType::ApartmentBuilding:
		r = new ApartmentBuilding();
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
