#include "Contract.h"


std::ostream &operator<<(std::ostream &out, const Contract *&r)
{
	out << static_cast<int>(r->get_type()) << ',';
	return r->output(out);
}

std::istream &operator>>(std::istream &in, Contract *&r)
{
	ContractType type;
	in >> reinterpret_cast<int&>(type);
	in.ignore();
	switch (type)
	{
	case ContractType::Rent:
		r = new RentConstract();
		break;
	case ContractType::Sale:
		r = new SaleContract();
		break;
	default:
		throw std::logic_error("");
	}
	return r->input(in);
}
