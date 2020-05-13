#pragma once

#include <memory>
#include <string>

class Expression
{
public:
    virtual ~Expression() = default;

    virtual std::string print() = 0;
};

class I32Literal : public Expression
{
public:
    I32Literal(int value);

    std::string print();

private:
    int _value;
};

class Identifier : public Expression
{
public:
    Identifier(std::string identifier);

    std::string print();

private:
    std::string _identifier;
};

class BinaryOp : public Expression
{
public:
    BinaryOp(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);

    virtual std::string print() = 0;
    
protected:
    std::unique_ptr<Expression> _lhs;
    std::unique_ptr<Expression> _rhs;
};

class Addition : public BinaryOp
{
public:
    Addition(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);

    std::string print();
};

class Subtraction : public BinaryOp
{
public:
    Subtraction(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);

    std::string print();
};

class Multiplication : public BinaryOp
{
public:
    Multiplication(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);

    std::string print();
};

class Division : public BinaryOp
{
public:
    Division(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);

    std::string print();
};