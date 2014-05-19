#vecvec.thrift

struct Feature
{
1: string name;
2: list<double> content;
}

struct VecVec
{   
1: list< Feature> content;
}