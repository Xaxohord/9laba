#include "../inc/FunctionsHeader.hpp"
#include "../inc/mathutils/matrix.hpp"

//using namespace mtmath;
BMP::BMP() {
	m_width = 0;
	m_height = 0;
	//int inclass_height = 0;
}
BMP::BMP(int h, int w){
	m_width = w;
	m_height = h;
	m_pixels = new Pixel *[m_height];
	for (int i = 0; i < m_height; i++)
		m_pixels[i] = new Pixel[m_width];
	m_coordinates = new Vec2d *[m_height];
	for (int i = 0; i < m_height; i++)
		m_coordinates[i] = new Vec2d[m_width];

	for (int i = 0; i < m_height; i++)
		for (int j = 0; j < m_width; j++)
			m_pixels[i][j] = { 0,0,0 };

	for (int i = 0; i < m_height; i++)
		for (int j = 0; j < m_width; j++)
		{
			m_coordinates[i][j].set(0, 0, j);
			m_coordinates[i][j].set(1, 0, i);
		}

	for (int i = 0; i < m_height; i++)
		for (int j = 0; j < m_width; j++)
			m_pixels[i][j] = { 0,0,0 };
}
void BMP::Read() {
    // Чтение файла
    std::ifstream in("in.bmp", std::ios::binary);
    in.read(reinterpret_cast<char*>(&bmpHeader), sizeof(BMPHEADER));
    in.read(reinterpret_cast<char*>(&bmpInfo), sizeof(BMPINFO));
   // m_width = bmpInfo.Width;
   // m_height = bmpInfo.Height;
	//std::ifstream in("in.bmp", std::ios::binary);
	m_pixels = new Pixel *[m_height];
	for (int i = 0; i < m_height; i++)
		m_pixels[i] = new Pixel[m_width];

	for (int i = 0; i < bmpInfo.Height; i++)
	{
		for (int j = 0; j < bmpInfo.Width; j++)
			in.read(reinterpret_cast<char*>(&m_pixels[i][j]), sizeof(Pixel));

		if ((3 * bmpInfo.Width) % 4 != 0)
			for (int j = 0; j < 4 - (3 * bmpInfo.Width) % 4; j++)
			{
				char c;
				in.read(&c, 1);
			}
	}

}
//void BPM::Size()
/*void BMP::Pixels() {
    std::ifstream in("in.bmp", std::ios::binary);
    m_pixels = new Pixel * [m_height];
    for (int i = 0; i < m_height; i++)
        m_pixels[i] = new Pixel[m_width];

    for (int i = 0; i < bmpInfo.Height; i++)
    {
        for (int j = 0; j < bmpInfo.Width; j++)
            in.read(reinterpret_cast<char*>(&m_pixels[i][j]), sizeof(Pixel));

        if ((3 * bmpInfo.Width) % 4 != 0)
            for (int j = 0; j < 4 - (3 * bmpInfo.Width) % 4; j++)
            {
                char c;
                in.read(&c, 1);
            }
    }
}*/

void BMP::DarkFilter() {
    for (int i = 0; i < bmpInfo.Height; i++)
        for (int j = 0; j < bmpInfo.Width; j++)
        {
            if (m_pixels[i][j].b - 80 > 0)
                m_pixels[i][j].b -= 80;
            if (m_pixels[i][j].g - 80 > 0)
                m_pixels[i][j].g -= 80;
            if (m_pixels[i][j].r - 80 > 0)
                m_pixels[i][j].r -= 80;
        }
}
void BMP::Rotate(double angle)
{
	// 1.   
	Vec2d T({ {
		{(double)(m_width / 2)},
		{(double)(m_height / 2)}
	} });

	for (int i = 0; i < m_height; i++)
		for (int j = 0; j < m_width; j++)
			m_coordinates[i][j] = m_coordinates[i][j] - T;

	// 2. 
	Mat22d R({ {
		{cos(angle), sin(angle)},
		{-sin(angle), cos(angle)}
	} });

	for (int i = 0; i < m_height; i++)
		for (int j = 0; j < m_width; j++)
		{
			m_coordinates[i][j] = R * m_coordinates[i][j];
			//std::cout << m_coordinates[i][j] << std::endl;
		}

	// 3.      
	int maxX = INT_MIN;
	int minX = INT_MAX;
	int maxY = INT_MIN;
	int minY = INT_MAX;
	for (int i = 0; i < m_height; i++)
		for (int j = 0; j < m_width; j++)
		{
			if (maxX < m_coordinates[i][j].get(0, 0))
				maxX = m_coordinates[i][j].get(0, 0);
			if (minX > m_coordinates[i][j].get(0, 0))
				minX = m_coordinates[i][j].get(0, 0);
			if (maxY < m_coordinates[i][j].get(1, 0))
				maxY = m_coordinates[i][j].get(1, 0);
			if (minY > m_coordinates[i][j].get(1, 0))
				minY = m_coordinates[i][j].get(1, 0);
		}

	//       -  
	maxX++;
	minX--;
	maxY++;
	minY--;
	//std::cout << maxX << " " << minX<<std::endl;
	//std::cout << maxY << " " << minY<<std::endl;
	//system("pause");
	int width =  maxX - minX;
	int height =  maxY - minY;

	//     
	Vec2d shift({ {
		{(double)(width / 2)},
		{(double)(height / 2)}
	} });

	for (int i = 0; i < m_height; i++)
		for (int j = 0; j < m_width; j++)
			m_coordinates[i][j] = m_coordinates[i][j] + shift;

	//   
	Pixel** new_pixels = new Pixel *[height];
	for (int i = 0; i < height; i++)
		new_pixels[i] = new Pixel[width];

	Vec2d** new_coordinates = new Vec2d *[height];
	for (int i = 0; i < height; i++)
		new_coordinates[i] = new Vec2d[width];

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			new_pixels[i][j] = { 0,0,0 };

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			new_coordinates[i][j].set(0, 0, j);
			new_coordinates[i][j].set(0, 0, i);
		}

	//  
	for (int i = 0; i < m_height; i++)
		for (int j = 0; j < m_width; j++)
		{
			int x = (int)(m_coordinates[i][j].get(0, 0));
			int y = (int)(m_coordinates[i][j].get(1, 0));
			new_pixels[y][x] = m_pixels[i][j];
		}

	//   
	for (int i = 0; i < m_height; i++)
		delete[] m_pixels[i];
	delete[] m_pixels;

	for (int i = 0; i < m_height; i++)
		delete[] m_coordinates[i];
	delete[] m_coordinates;

	m_pixels = new_pixels;
	m_coordinates = new_coordinates;

	m_width = width;
	m_height = height;
}

void BMP::Write() {
    std::ofstream out("out.bmp", std::ios::binary);
    //m_width = bmpInfo.Width;
    //m_height = bmpInfo.Height;

    // Формирование заголовка
    BMPHEADER bmpHeader_new;
    bmpHeader_new.Type = 0x4D42; // Тип данных BMP
    bmpHeader_new.Size = 14 + 40 + (3 * m_width * m_height);
    if (m_width % 4 != 0)
        bmpHeader_new.Size += (4 - (3 * m_width) % 4) * m_height;
    bmpHeader_new.OffBits = 54;
    bmpHeader_new.Reserved1 = 0;
    bmpHeader_new.Reserved2 = 0;

    out.write(reinterpret_cast<char*>(&bmpHeader_new), sizeof(BMPHEADER));

    // Формирование информации об изображении
    BMPINFO bmpInfo_new;
    bmpInfo_new.BitCount = 24;
    bmpInfo_new.ClrImportant = 0;
    bmpInfo_new.ClrUsed = 0;
    bmpInfo_new.Compression = 0;
    bmpInfo_new.Height = m_height;
    bmpInfo_new.Planes = 1;
    bmpInfo_new.Size = 40;
    bmpInfo_new.SizeImage = bmpHeader_new.Size - 54;
    bmpInfo_new.Width = m_width;
    bmpInfo_new.XPelsPerMeter = 0;
    bmpInfo_new.YPelsPerMeter = 0;

    out.write(reinterpret_cast<char*>(&bmpInfo_new), sizeof(BMPINFO));

    // Записать пиксели
    for (int i = 0; i < bmpInfo_new.Height; i++)
    {
        for (int j = 0; j < bmpInfo_new.Width; j++)
            out.write(reinterpret_cast<char*>(&m_pixels[i][j]), sizeof(Pixel));

        if ((3 * bmpInfo_new.Width) % 4 != 0)
            for (int j = 0; j < 4 - (3 * bmpInfo_new.Width) % 4; j++)
            {
                char c = 0;
                out.write(&c, 1);
            }
    }
}
void BMP::Correct() {
	for (int i = 1; i < m_width - 1; i++) {
		for (int j = 1; j < m_height - 1; j++) {
			if (m_pixels[i][j].b + m_pixels[i][j].g + m_pixels[i][j].r == 0) {
				int z = 0;
				if (m_pixels[i - 1][j - 1].b + m_pixels[i - 1][j - 1].g + m_pixels[i - 1][j - 1].r > 0)
					z += 1;																		   
				if (m_pixels[i - 1][j - 0].b + m_pixels[i - 1][j - 0].g + m_pixels[i - 1][j - 0].r > 0)
					z += 1;																		   
				if (m_pixels[i - 1][j + 1].b + m_pixels[i - 1][j + 1].g + m_pixels[i - 1][j + 1].r > 0)
					z += 1;																		   
				if (m_pixels[i - 0][j - 1].b + m_pixels[i - 0][j - 1].g + m_pixels[i - 0][j - 1].r > 0)
					z += 1;																		   
				if (m_pixels[i - 0][j + 1].b + m_pixels[i - 0][j + 1].g + m_pixels[i - 0][j + 1].r > 0)
					z += 1;																		   
				if (m_pixels[i + 1][j - 1].b + m_pixels[i + 1][j - 1].g + m_pixels[i + 1][j - 1].r > 0)
					z += 1;																		   
				if (m_pixels[i + 1][j - 0].b + m_pixels[i + 1][j - 0].g + m_pixels[i + 1][j - 0].r > 0)
					z += 1;																		   
				if (m_pixels[i + 1][j + 1].b + m_pixels[i + 1][j + 1].g + m_pixels[i + 1][j + 1].r > 0)
					z += 1;
				if (z > 4) {
					int blueCol = m_pixels[i - 1][j - 1].b + m_pixels[i - 1][j - 0].b + m_pixels[i - 1][j + 1].b + m_pixels[i - 0][j - 1].b + m_pixels[i - 0][j + 1].b + m_pixels[i + 1][j - 1].b + m_pixels[i + 1][j - 0].b + m_pixels[i + 1][j + 1].b;
					blueCol /= z;
					int greenCol = m_pixels[i - 1][j - 1].g + m_pixels[i - 1][j - 0].g + m_pixels[i - 1][j + 1].g + m_pixels[i - 0][j - 1].g + m_pixels[i - 0][j + 1].g + m_pixels[i + 1][j - 1].g + m_pixels[i + 1][j - 0].g + m_pixels[i + 1][j + 1].g;
					greenCol /= z;
					int readCol = m_pixels[i - 1][j - 1].r + m_pixels[i - 1][j - 0].r + m_pixels[i - 1][j + 1].r + m_pixels[i - 0][j - 1].r + m_pixels[i - 0][j + 1].r + m_pixels[i + 1][j - 1].r + m_pixels[i + 1][j - 0].r + m_pixels[i + 1][j + 1].r;
					readCol /= z;
					//z = 0;
					m_pixels[i][j].b = blueCol;
					m_pixels[i][j].g = greenCol;
					m_pixels[i][j].r = readCol;
				}
				
			}
		}
	}
}
void BMP::Clean() {
    for (int i = 0; i < m_height; i++)
        delete[] m_pixels[i];
    delete[] m_pixels;
}