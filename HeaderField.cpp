#include "HeaderField.hpp"

HeaderField::HeaderField()
:_name(),
_val()
{
}

HeaderField::HeaderField(
	const std::string & name
)
:_name(name)
{
}

HeaderField::HeaderField(
	const std::string & name,
	const std::string & val
)
:_name(name),
_val(val)
{
}

HeaderField::HeaderField(
	const HeaderField & another
)
:_name(another._name),
_val(another._val)
{
}

HeaderField::~HeaderField()
{
}

const std::string & HeaderField::getName() const {
	return _name;
}

const std::string & HeaderField::getValue() const {
	return _val;
}

HeaderField & HeaderField::setName(const std::string & name)
{
	_name = name;
	return *this;
}

HeaderField & HeaderField::setValue(const std::string & val)
{
	_val = val;
	return *this;
}
