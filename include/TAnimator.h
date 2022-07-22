#ifndef _TELLER_H_TANIMATOR_
#define _TELLER_H_TANIMATOR_

#include"cinder/gl/gl.h"
#include"nlohmann/json.hpp"

namespace teller {

	/// <summary>
	/// 
	/// </summary>
	class TAnimatorBase {
	private:
	protected:
		float elapsedTime = 0.f;
	public:
		TAnimatorBase() = default;
		virtual ~TAnimatorBase() = default;

		TAnimatorBase(const TAnimatorBase&) = delete;
		TAnimatorBase& operator=(const TAnimatorBase&) = delete;
		TAnimatorBase& operator=(TAnimatorBase&&) = default;

		virtual void Update(float& deltaTime) = 0;
	};

	class TAnimatorCircular :public TAnimatorBase {
	private:
	protected:
	public:
		TAnimatorCircular() = default;
		~TAnimatorCircular() = default;

		void Update(float& deltaTime) override;
	};
}

#endif 