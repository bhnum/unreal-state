#include "Contract.h"


std::ostream &operator<<(std::ostream &out, const Contract *r)
{
	out << static_cast<int>(r->get_type()) << ',';
	r->output(out);
	return out;
}

std::istream &operator>>(std::istream &in, Contract *&r)
{
	ContractType type;
	in >> reinterpret_cast<int&>(type);
	in.ignore();
	switch (type)
	{
	case ContractType::Rent:
		r = new RentContract();
		break;
	case ContractType::Sale:
		r = new SaleContract();
		break;
	default:
		if (in.eof())
			return in;
		throw std::logic_error("");
	}
	r->input(in);
	return in;
}
