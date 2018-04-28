#include <Siv3D.hpp>

Size tileSize(10, 10);
Point getRandomPoint(double radius)
{
	const double theta = 2 * Math::Pi*Random();
	double r = 0;
	for (int i = 0; i < 5; i++)
		r += radius*Random();
	r /= 5;
	return Point(tileSize.x*ceil(r*cos(theta) / tileSize.x+1), tileSize.y*ceil(r*sin(theta) / tileSize.y+1));
}

void addRect(int r, std::vector<Rect>& rooms)
{
	while (true)
	{
		const Point p1 = getRandomPoint(r);
		const Point p2 = getRandomPoint(r);
		const Size size(abs((p1 - p2).x), abs((p1 - p2).y));
		Rect re(p1, size);

		if ((Circle(r)).contains(re))
		{
			rooms.emplace_back(re);
			return;
		}
	}
}

void Main()
{
	Window::Resize(1000, 1000);
	const int r = 200;
	const Circle circle(r);


	const int roomNum = 50;
	std::vector<Rect> roomRect;
	for (int i = 0; i < roomNum; i++)
		addRect(r, roomRect);

	Point centerPos = Window::Center();
	size_t nowNumber = 0;
	bool next = false;

	while (System::Update())
	{
		if (Input::MouseL.clicked)
			centerPos = Mouse::Pos();

		int k = 0;
		for (size_t i = 0; i < roomNum; i++)
		{
			if (roomRect[nowNumber].intersects(roomRect[i]) && i != nowNumber)
			{
				if (abs(roomRect[nowNumber].center.x) > abs(roomRect[nowNumber].center.y))
					roomRect[nowNumber].moveBy(roomRect[nowNumber].center.x > 0 ? tileSize.x : -tileSize.x, 0);// roomRect[nowNumber].center.y > 0 ? tileSize.y : -tileSize.y);
				else
					roomRect[nowNumber].moveBy(0,roomRect[nowNumber].center.x > 0 ? tileSize.y : -tileSize.y);// roomRect[nowNumber].center.y > 0 ? tileSize.y : -tileSize.y);
				
				k++;
			}
		}

		if (k == 0 && nowNumber != roomNum - 1)
		{
			nowNumber = (nowNumber + 1) % roomNum;
			next = false;
		}


		{
			const Transformer2D transformer(Mat3x2::Scale(0.5).translate(centerPos));
			//circle.drawFrame(2.0, 0.0, Color(Palette::White, 200));
			for (auto e : roomRect)
			{
				if (e == roomRect[nowNumber])
					e.draw(Color(Palette::Tomato, 100)).drawFrame(1.0, 0.0, Palette::Red);
				else
					e.draw(Color(Palette::Aqua, 20)).drawFrame(1.0, 0.0, Palette::Aliceblue);
			}
		}

		if (Input::KeyEnter.clicked)
		{
			nowNumber = 0;
			roomRect.clear();
			for (int i = 0; i < roomNum; i++)
				addRect(r, roomRect);
		}
	}
}