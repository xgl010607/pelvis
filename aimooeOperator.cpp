#include "aimooeOperator.h"

aimooeOperator::aimooeOperator(QObject* parent) :QThread(parent) {}

void aimooeOperator::openAimooe() {
	int rt = 1;
	Aim_API_Initial(aimHandle);
	EI = I_USB; oldEI = I_USB;
	useUSBorEthernetorWIFI = 0;
	switch (useUSBorEthernetorWIFI)
	{
	case 0: {EI = I_USB; oldEI = I_USB; }break;
	case 1: {EI = I_ETHERNET; oldEI = I_ETHERNET; }break;
	case 2: {EI = I_WIFI; oldEI = I_WIFI; }break;
	default: {std::cout << "输入有误！" << endl;  }break;
	}
	std::cout << "AR demo启动" << std::endl;
	string strversion;

	int number;

	//char * path = "C:\\AimTools\\";
	char** toolarr = NULL;



	if (EI == I_ETHERNET)
	{
		//App-100
		//Aim_SetEthernetConnectIP(192, 168,1,10);
		//App-200
		Aim_SetEthernetConnectIP(aimHandle, 192, 168, 31, 10);
	}
	E_ReturnValue rlt = Aim_ConnectDevice(aimHandle, EI, mPosDataPara);
	/*int graywidth = mPosDataPara.dualimg.width;
	int grayheight = mPosDataPara.dualimg.height;
	ImageLeft = new char[graywidth * grayheight];
	ImageRight = new char[graywidth * grayheight];*/
#if GET_DATA_FROM_HARDWARE 
	Aim_SetAcquireData(aimHandle, EI, DT_NONE);
#else
	Aim_SetAcquireData(aimHandle, EI, DT_INFO);
#endif
	if (rlt == AIMOOE_OK)
	{
		//Aim_SetMarkerParameters(aimHandle, EI, 75,1000,80);
		std::cout << "连接成功！" << endl;
		oldEI = EI;
		toolidarr.push_back("pin");
		toolidarr.push_back("base");
		char* c = const_cast<char*>(patht);
		rlt = Aim_SetToolInfoFilePath(aimHandle, c);
		int rlt1;
#if GET_DATA_FROM_HARDWARE 
		Aim_SetAcquireData(aimHandle, EI, DT_NONE);
#else
		rlt1 = Aim_SetAcquireData(aimHandle, EI, DT_INFO);
#endif
		if (rlt1 == AIMOOE_OK && rlt == AIMOOE_OK) {
			emit flage(true);
			isStarted = true;
		}
	}
}
void aimooeOperator::captureGrayFrame() {
	int rlt;
	int graywidth = mPosDataPara.dualimg.width;
	int grayheight = mPosDataPara.dualimg.height;
	iL = new char[graywidth * grayheight];
	iR = new char[graywidth * grayheight];
	rlt = Aim_SetAcquireData(aimHandle, EI, DT_IMGDUAL);

	rlt = Aim_GetGreyImageDual(aimHandle, EI, iL, iR);
	emit ImageLeft(iL);
	emit ImageRight(iR);
}
void aimooeOperator::captureColorFrame() {
	int rlt;
	int colorwidth = mPosDataPara.colorimg.width;
	int colorheight = mPosDataPara.colorimg.height;
	iC = new char[colorwidth * colorheight * 2];
	rlt = Aim_SetAcquireData(aimHandle, EI, DT_INFO_IMGCOLOR);

	rlt = Aim_GetColorImageMiddle(aimHandle, EI, iC);
	emit ImageColor(iC);
}
void aimooeOperator::loopGetTool() {
	while (1) {

#if GET_DATA_FROM_HARDWARE 
		rlt = Aim_GetMarkerAndStatusFromHardware(aimHandle, EI, markerSt, statusSt);
#else
		rlt = Aim_GetMarkerInfo(aimHandle, EI, markerSt);
#endif	
		if (rlt != AIMOOE_OK)
			continue;
		T_AimToolDataResult* mtoolsrlt = new T_AimToolDataResult;
		mtoolsrlt->next = NULL;
		rlt = Aim_FindSpecificToolInfo(aimHandle, markerSt, this->toolidarr, mtoolsrlt, 3);
		
		T_AimToolDataResult* prlt = mtoolsrlt;
		if (rlt == AIMOOE_OK)
		{
			do
			{
				if (prlt->validflag)
				{
					float PI = 3.141592;
					QString add_text = QString::number(prlt->OriginCoor[0]);
					add_text += " ";
					add_text += QString::number(prlt->OriginCoor[1]);
					add_text += " ";
					add_text += QString::number(prlt->OriginCoor[2]);
					add_text += " ";
					add_text += QString::number(prlt->rotationvector[0] * 180 / PI);
					add_text += " ";
					add_text += QString::number(prlt->rotationvector[1] * 180 / PI);
					add_text += " ";
					add_text += QString::number(prlt->rotationvector[2] * 180 / PI);
					add_text += "\n";
					emit RTSignal(add_text);
					/*for (int i = 0; i < 3; i++) {
						for (int j = 0; j < 3; j++) {
							RT[i][j] = prlt->Rto[i][j];
						}
						RT[i][3] = prlt->OriginCoor[i];
					}
					RT[3][3] = 1;
					*/

				}
				T_AimToolDataResult* pnext = prlt->next;
				delete prlt;
				prlt = pnext;
			} while (prlt != NULL);
		}
		else
		{
			delete prlt;
		}
	}
}
void aimooeOperator::GetToolInfo(std::vector<toolPosition> &toolList) {

#if GET_DATA_FROM_HARDWARE 
	rlt = Aim_GetMarkerAndStatusFromHardware(aimHandle, EI, markerSt, statusSt);
#else
	rlt = Aim_GetMarkerInfo(aimHandle, EI, markerSt);
#endif	
	if (rlt != AIMOOE_OK)
		return;
	T_AimToolDataResult* mtoolsrlt = new T_AimToolDataResult;
	mtoolsrlt->next = NULL;
	rlt = Aim_FindSpecificToolInfo(aimHandle, markerSt, this->toolidarr, mtoolsrlt, 3);

	T_AimToolDataResult* prlt = mtoolsrlt;
	if (rlt == AIMOOE_OK)
	{
		do
		{
			if (prlt->validflag)
			{
				float PI = 3.141592;
				
				toolPosition tP;
				tP.name = prlt->toolname;
				tP.qR[0] = prlt->Qoxyz[0];
				tP.qR[1] = prlt->Qoxyz[1];
				tP.qR[2] = prlt->Qoxyz[2];
				tP.qR[3] = prlt->Qoxyz[3];//未作硬件设备映射
				tP.vR[0] = prlt->rotationvector[0] - PI / 2;
				tP.vR[1] = prlt->rotationvector[1];
				tP.vR[2] = prlt->rotationvector[2] - PI / 2;
			
				for(int i=0;i<3;i++)
					for(int j=0;j<3;j++)
						tP.Rto[i][j] = prlt->Rto[i][j];
				tP.Tto[0] = prlt->Tto[0];
				tP.Tto[1] = prlt->Tto[1];
				tP.Tto[2] = prlt->Tto[2];
				toolList.push_back(tP);
			}
			T_AimToolDataResult* pnext = prlt->next;
			delete prlt;
			prlt = pnext;
		} while (prlt != NULL);
	}
	else
	{
		delete prlt;
	}

	
}

void aimooeOperator::GetMarkerInfo() {
	int rlt;
#if GET_DATA_FROM_HARDWARE 
	rlt = Aim_GetMarkerAndStatusFromHardware(aimHandle, EI, markerSt, statusSt);
#else
	rlt = Aim_GetMarkerInfo(aimHandle, EI, markerSt);
#endif	
	if (rlt == AIMOOE_OK)
	{
		int markerNum = markerSt.MarkerNumber;
		std::cout << "开机后第" << markerSt.ID << "次采集" << endl;
		std::cout << "标记点背景光(1:正常，0：不正常)：" << (markerSt.MarkerBGLightStatus == BG_LIGHT_OK) << endl;
		std::cout << "检测到标记点数量：" << markerNum << endl;
		Eigen::Vector3d massPoint;
		Eigen::Vector3d normalPlan;
		Eigen::Vector3d rotationD;
		for (int i = 0; i < markerNum; i++)
		{
			std::cout << i << ":(" << markerSt.MarkerCoordinate[i * 3 + 0] << ", " << markerSt.MarkerCoordinate[i * 3 + 1] << ", " << markerSt.MarkerCoordinate[i * 3 + 2] << ")" << endl;

		}

	}
}

void aimooeOperator::write_bmpheader(unsigned char* bitmap, int offset, int bytes, int value)
{
	int i;
	for (i = 0; i < bytes; i++)
		bitmap[offset + i] = (value >> (i << 3)) & 0xFF;
}
unsigned char* aimooeOperator::convertGrayTo24bitBmp(unsigned char* inputImg, int width, int height, int* ouputSize)
{
	/*create a bmp format file*/
	int bitmap_x = (int)ceil((double)width * 3 / 4) * 4;
	unsigned char* bitmap = (unsigned char*)malloc(sizeof(unsigned char) * height * bitmap_x + 54);

	bitmap[0] = 'B';
	bitmap[1] = 'M';
	write_bmpheader(bitmap, 2, 4, height * bitmap_x + 54); //whole file size
	write_bmpheader(bitmap, 0xA, 4, 54); //offset before bitmap raw data
	write_bmpheader(bitmap, 0xE, 4, 40); //length of bitmap info header
	write_bmpheader(bitmap, 0x12, 4, width); //width
	write_bmpheader(bitmap, 0x16, 4, height); //height
	write_bmpheader(bitmap, 0x1A, 2, 1);
	write_bmpheader(bitmap, 0x1C, 2, 24); //bit per pixel
	write_bmpheader(bitmap, 0x1E, 4, 0); //compression
	write_bmpheader(bitmap, 0x22, 4, height * bitmap_x); //size of bitmap raw data
	for (int i = 0x26; i < 0x36; i++)
		bitmap[i] = 0;

	int k = 54;

	for (int i = height - 1; i >= 0; i--) //bmp图像存储从最后一行开始
	{
		int j;
		for (j = 0; j < width; j++)
		{
			int index = i * width + j;
			bitmap[k++] = inputImg[index];
			bitmap[k++] = inputImg[index];
			bitmap[k++] = inputImg[index];
		}
		j *= 3;
		while (j < bitmap_x) {
			bitmap[k++] = 0;
			j++;
		}
	}

	*ouputSize = k;
	return bitmap;
}
unsigned char* aimooeOperator::convertColor16To24bitBmp(unsigned char* inputImg, int width, int height, int* ouputSize)
{
	/*create a bmp format file*/
	int bitmap_x = (int)ceil((double)width * 3 / 4) * 4;
	unsigned char* bitmap = (unsigned char*)malloc(sizeof(unsigned char) * height * bitmap_x + 54);

	bitmap[0] = 'B';
	bitmap[1] = 'M';
	write_bmpheader(bitmap, 2, 4, height * bitmap_x + 54); //whole file size
	write_bmpheader(bitmap, 0xA, 4, 54); //offset before bitmap raw data
	write_bmpheader(bitmap, 0xE, 4, 40); //length of bitmap info header
	write_bmpheader(bitmap, 0x12, 4, width); //width
	write_bmpheader(bitmap, 0x16, 4, height); //height
	write_bmpheader(bitmap, 0x1A, 2, 1);
	write_bmpheader(bitmap, 0x1C, 2, 24); //bit per pixel
	write_bmpheader(bitmap, 0x1E, 4, 0); //compression
	write_bmpheader(bitmap, 0x22, 4, height * bitmap_x); //size of bitmap raw data
	for (int i = 0x26; i < 0x36; i++)
		bitmap[i] = 0;

	int k = 54;

	for (int i = height - 1; i >= 0; i--) //bmp图像存储从最后一行开始
	{
		int j;
		for (j = 0; j < width; j++)
		{
			int index = (i * width + j) * 2;
			bitmap[k++] = (inputImg[index] << 3);//blue
			bitmap[k++] = ((inputImg[index + 1] << 5) | (inputImg[index] >> 3)) & 0xFC;//green
			bitmap[k++] = inputImg[index + 1] & 0xF8;//red
		}
		j *= 3;
		while (j < bitmap_x) {
			bitmap[k++] = 0;
			j++;
		}
	}

	*ouputSize = k;
	return bitmap;
}
bool aimooeOperator::saveToBmp(unsigned char* inputImg, int width, int height, char* outputFileName, E_BMPBIT bmpBit)
{
	int size;
	unsigned char* bmp;
	if (bmpBit == BMP8BIT)
		bmp = convertGrayTo24bitBmp(inputImg, width, height, &size);
	else
		bmp = convertColor16To24bitBmp(inputImg, width, height, &size);
	FILE* fp = fopen(outputFileName, "wb+");
	if (fp == NULL) {
		return false;
	}
	fwrite(bmp, 1, size, fp);
	fclose(fp);
	free(bmp);

	return true;
}
void aimooeOperator::Ipl2VTK(cv::Mat& src, vtkImageData* dst)
{
	cv::flip(src, src, 0);
	vtkImageImport* importer = vtkImageImport::New();
	importer->SetOutput(dst);
	importer->SetDataSpacing(1, 1, 1);
	importer->SetDataOrigin(0, 0, 0);
	importer->SetWholeExtent(0, src.size().width - 1, 0, src.size().height - 1, 0, 0);
	importer->SetDataExtentToWholeExtent();
	importer->SetDataScalarTypeToUnsignedChar();
	importer->SetNumberOfScalarComponents(src.channels());
	importer->SetImportVoidPointer(src.data);
	importer->Update();
	return;
}
