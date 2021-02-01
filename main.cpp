/**
 * Савченко И.В.
 * М8О-208Б-19
 * https://github.com/ShyFly46/oop_excercise_04
 * Вариант 24:
 * * Квадрат
 * * Треугольник
 * * Восьмиугольник
 **/

#include <iostream>
#include <type_traits>
#include <cmath>
#include <tuple>

#include <utility>
#include <string>

template <typename T>
struct Triangle {
	std::pair<T, T> points[3];

	std::string name = "Triangle";
};

template <typename T>
struct Square {
    std::pair<T, T> points[4];
    const std::string name = "Square";
};

template <typename T>
struct Octagon {
    std::pair<T, T> points[8];

    std::string name = "Octagon";
};

using coord_t = double;

template<typename T, class R>
typename std::enable_if<std::is_same<T, Square<R>&>::value, bool>::type
readFigure(T figure) {
	std::cout << "Suare, 2 points\n";
	std::cin >> figure.points[0].first >> figure.points[0].second
	         >> figure.points[1].first >> figure.points[1].second;

	std::pair<R, R> vec(
			figure.points[1].second - figure.points[0].second,
			figure.points[0].first - figure.points[1].first
			);

	figure.points[3].first  = figure.points[0].first  + vec.first;
	figure.points[3].second = figure.points[0].second + vec.second;

	figure.points[2].first  = figure.points[1].first + vec.first;
	figure.points[2].second  = figure.points[1].second + vec.second;

	return true;
}

template<typename T, class R>
typename std::enable_if<std::is_same<T, Octagon<R>&>::value, bool>::type
readFigure(T figure) {
	std::cout << "Octagon, 2 points\n";
	std::pair<R, R> centre;
	std::cin >> centre.first
		     >> centre.second;
	std::cin >> figure.points[0].first
		     >> figure.points[0].second;

	std::pair<R, R> vec;
	vec.first = figure.points[0].first - centre.first;
	vec.second = figure.points[0].second - centre.second;

	figure.points[4].first = centre.first - vec.first;
	figure.points[4].second = centre.second - vec.second;

	R temp = vec.second;
	vec.second = -vec.first;
	vec.first = temp;

	figure.points[2].first = centre.first + vec.first;
	figure.points[2].second = centre.second + vec.second;

	figure.points[6].first = centre.first - vec.first;
	figure.points[6].second = centre.second - vec.second;

	const double r2 = 1 / std::sqrt(2);
	std::pair<R, R> dia(
			(vec.first + vec.second) * r2,
			(vec.second - vec.first) * r2
			);

	figure.points[3].first = centre.first + dia.first;
	figure.points[3].second = centre.second + dia.second;

	figure.points[7].first = centre.first - dia.first;
	figure.points[7].second = centre.second - dia.second;

	temp = dia.second;
	dia.second = -dia.first;
	dia.first = temp;

	figure.points[5].first = centre.first + dia.first;
	figure.points[5].second = centre.second + dia.second;

	figure.points[1].first = centre.first - dia.first;
	figure.points[1].second = centre.second - dia.second;

	return true;
}

template<typename T, class R>
typename std::enable_if<std::is_same<T, Triangle<R>&>::value, bool>::type
readFigure(T figure) {
	std::cout << "Triangle, 3 points\n";
	for(int i = 0; i < 3; ++i){
		std::cin >> figure.points[i].first
			     >> figure.points[i].second;
	}

	return true;
}

template<typename T, size_t index>
typename std::enable_if<index == std::tuple_size<T>::value, void>::type
printTuple(T &) {
}

template<typename T, size_t index>
typename std::enable_if<index < std::tuple_size<T>::value, void>::type
printTuple(T &t) {
    auto figure = std::get<index>(t);
    std::cout << "Vertices of " << figure.name << ":\t";
    printVertices(figure);
    printTuple<T, index + 1>(t);
}

template<typename T>
typename std::enable_if<(sizeof(T::points) / sizeof(T::points[0]) > 0), void>::type
printVertices(T &figure) {
    for (auto v : figure.points)
        std::cout << " ( " << v.first << ", " << v.second << " ) ";
    std::cout << '\n';
}


template<class T, size_t index>
double totalTupleArea(T &tuple) {
    auto figure = std::get<index>(tuple);
    coord_t value = figureArea(figure);
    if constexpr ((index + 1) < std::tuple_size<T>::value) {
        return value + totalTupleArea<T, index + 1>(tuple);
    }
    return value;
}

template<class R>
double triangleArea(
		std::pair<R, R> const &first,
		std::pair<R, R> const &second,
		std::pair<R, R> const &third)
{
	// a x
	// b y
	double a = second.first - first.first;
	double b = second.second - first.second;

	double x = third.first - first.first;
	double y = third.second - first.second;

	return std::fabs((a*y - x*b) * 0.5);	
}

template<class T>
double figureArea(T &figure) {
    double area = 0;
    int n = sizeof(T::points) / sizeof(T::points[0]);
    for (size_t i = 2; i < n; ++i)
		area += triangleArea(
				figure.points[0],
				figure.points[i-1],
				figure.points[i]
				);
	return area;
}

void help(){
	std::cout << "to add:\ns - square\nt - triangle\no - octagon\n";
	std::cout << "---\nv - print vertices\na - total area\n";
}

int main() {
    std::tuple<Square<double>, Triangle<double>, Octagon<double>> tp;

	help();
	char cmd;
	std::cout << ">";
	while(std::cin >> cmd){
		switch (cmd) {
			case 's': // add square
			{
				auto &fig = std::get<0>(tp);
				if(readFigure<decltype(fig), coord_t>(fig))
					std::cout << std::get<0>(tp).name << " successfully added\n";
				break;
			}
			case 't': // add triangle
			{
				auto &fig = std::get<1>(tp);
				if(readFigure<decltype(fig), coord_t>(fig))
					std::cout << std::get<1>(tp).name << " successfully added\n";
				break;
			}
			case 'o': // add octagon
			{
				auto &fig = std::get<2>(tp);
				if(readFigure<decltype(fig), coord_t>(fig))
					std::cout << std::get<2>(tp).name << " successfully added\n";
				break;
			}
			case 'v': { // all vertices
				printTuple<decltype(tp), 0>(tp);
				break;
			}
			case 'a': { // total area
				std::cout << "Total area of tuple\nS = " << totalTupleArea<decltype(tp), 0>(tp) << '\n';
				break;
			}
			default:{
				help();
				break;
			}
		}
		std::cout << ">";
	}

	std::cout << '\n';
    return 0;
}

