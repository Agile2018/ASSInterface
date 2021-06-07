#pragma once

#include <ASSInterface.h>
#include "idkit.h"

namespace ASSInterface {
	class InnoErrorIDkit : public ErrorIdentification {
	public:
		InnoErrorIDkit();
		~InnoErrorIDkit();
		virtual std::string GetError(int errorCode) override;
	};
}