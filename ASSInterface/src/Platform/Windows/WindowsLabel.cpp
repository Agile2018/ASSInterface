#include "hzpch.h"
#include "WindowsLabel.h"

namespace ASSInterface {
	Label* Label::s_Instance = new WindowsLabel();

	
	std::string WindowsLabel::GetLabelImpl(int index, LanguageType lg)
	{
		static bool init = false;

		if (!init)
		{
			init = true;
			Init();
		}

		int count = 0;

		std::vector<std::wstring> lgs = languages[index];		
		for (auto const& s : lgs) {
			if (count == (int)lg)
			{

				return CodeUTF8(s);
			}
			count++;
		}
		return "Not label";
	}

	void WindowsLabel::Init()
	{
		//Parameters

		languages.push_back({ L"Configuration of all parameters", L"Configuración de todos los parámetros" });
		languages.push_back({ L"Global parameters", L"Parámetros globales" });
		languages.push_back({ L"Face processing", L"Procesamiento facial" });
		languages.push_back({ L"Parameter defining threshold value for face confidence. Objects with lesser value are ignored(not counted).", L"Parámetro que define el valor umbral para la confianza facial. Los objetos de menor valor se ignoran (no se cuentan)." });
		languages.push_back({ L"Parameter defining how often often discovery (full frame face detection) frames appear (in milliseconds).", L"Parámetro que define la frecuencia con la que aparecen los fotogramas de descubrimiento (detección de rostros de fotograma completo) (en milisegundos)." });
		languages.push_back({ L"Parameter defining the maximum time difference(in milliseconds) between the disappearance of trajectory A and B.", L"Parámetro que define la diferencia de tiempo máxima(en milisegundos) entre la desaparición de la trayectoria A y B." });
		languages.push_back({ L"Parameter defining whether face entity is obtainable from tracked object in every video frame (true) or not (false).", L"Parámetro que define si la entidad facial se puede obtener a partir de un objeto rastreado en cada fotograma de video (verdadero) o no (falso)." });
		languages.push_back({ L"Classification of people for detection.", L"Clasificación de personas para la detección." });
		languages.push_back({ L"Channel number for the image stream.", L"Número de canal para el flujo de imágenes." });
		languages.push_back({ L"Source of image flow.", L"Origen de flujo de imágenes." });
		languages.push_back({ L"Channel window name.", L"Nombre de la ventana del canal." });
		languages.push_back({ L"Source type for the data flow.", L"Tipo de origen para el flujo de datos." });
		languages.push_back({ L"Channel number:", L"Número de canal:" });
		languages.push_back({ L"Face Detection Confidence Threshold:", L"Umbral de confianza de detección de rostros:" });
		languages.push_back({ L"Defines maximal size of detected faces. If it is in range (0, 1>) then it is relative to maximum of the input image dimensions. Otherwise it is considered as face size in pixels.", L"Define el tamaño máximo de las caras detectadas. Si está en el rango (0, 1>), entonces es relativo al máximo de las dimensiones de la imagen de entrada. De lo contrario, se considera el tamaño de la cara en píxeles." });
		languages.push_back({ L"Maximun Image Face Size:", L"Tamaño máximo de la cara de la imagen:" });
		languages.push_back({ L"Defines minimal size of detected faces. If it is in range (0, 1>) then it is relative to maximum of the input image dimensions. Otherwise it is considered as face size in pixels.", L"Define el tamaño mínimo de las caras detectadas. Si está en el rango (0, 1>), entonces es relativo al máximo de las dimensiones de la imagen de entrada. De lo contrario, se considera el tamaño de la cara en píxeles." });
		languages.push_back({ L"Minimun Face Image Size:", L"Tamaño mínimo de la imagen de la cara:" });
		languages.push_back({ L"Quality of the verification template, 0 - undefined, 1 - min quality, 255 - max quality.", L"Calidad de la plantilla de verificación, 0 - indefinida, 1 - calidad mínima, 255 - calidad máxima." });
		languages.push_back({ L"Template quality:", L"Calidad de la plantilla:" });
		languages.push_back({ L"Maximum count of faces to be detected.", L"Recuento máximo de rostros a detectar." });
		languages.push_back({ L"Maximun Faces Detected in Frame:", L"Caras máximas detectadas en el cuadro:" });
		languages.push_back({ L"Parameter defining face detection mode. It represents trade-off between face and facial features detection speed and accuracy.", L"Parámetro que define el modo de detección de rostros. Representa una compensación entre la velocidad y la precisión de detección de rasgos faciales." });
		languages.push_back({ L"Face Detection Speed Accuracy:", L"Precisión de la velocidad de detección de rostros:" });
		languages.push_back({ L"Parameter defining face template extraction mode. It represents trade-off between face template creation speed and face template quality.", L"Parámetro que define el modo de extracción de la plantilla facial. Representa una compensación entre la velocidad de creación de la plantilla facial y la calidad de la plantilla facial." });
		languages.push_back({ L"Face Template Extraction Speed Accuracy:", L"Precisión de la velocidad de extracción de la plantilla facial:" });
		languages.push_back({ L"Face can be cropped using one of listed methods.", L"La cara se puede recortar utilizando uno de los métodos enumerados." });
		languages.push_back({ L"Face Crop Full Frontal Scale:", L"Escala frontal completa de recorte facial:" });
	}

	std::string WindowsLabel::CodeUTF8(const std::wstring& wstr) {		
		if (wstr.empty()) return std::string("Not label");
		int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
		std::string strTo(size_needed, 0);
		WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
		return strTo;
	}

	LanguageType WindowsLabel::GetCurrentLanguageImpl()
	{
		LCID lcid = GetThreadLocale();
		wchar_t name[LOCALE_NAME_MAX_LENGTH];
		if (LCIDToLocaleName(lcid, name, LOCALE_NAME_MAX_LENGTH, 0) == 0) {
			ASS_ERROR("Error GetLocate: {0}", GetLastError());
			return LanguageType::enUs;
		}
		int size_needed = WideCharToMultiByte(CP_UTF8, 0, name, LOCALE_NAME_MAX_LENGTH, NULL, 0, NULL, NULL);
		std::string strTo(size_needed, 0);
		WideCharToMultiByte(CP_UTF8, 0, name, LOCALE_NAME_MAX_LENGTH, &strTo[0], size_needed, NULL, NULL);
		std::string lges("es-ES");
		std::size_t found = strTo.find(lges);
		if (found != std::string::npos)
			return LanguageType::esES;		
		//std::wcout << L"Locale name = " << name << std::endl;  es-ES   en-US

		return LanguageType::enUs;
	}
}