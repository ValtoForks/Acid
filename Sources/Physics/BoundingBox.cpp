#include "BoundingBox.hpp"

#include "Maths/Maths.hpp"

namespace fl
{
	BoundingBox::BoundingBox(const Vector3 &minExtents, const Vector3 &maxExtents) :
		IBounding(),
		m_minExtents(minExtents),
		m_maxExtents(maxExtents)
	{
	}

	BoundingBox::BoundingBox(const BoundingBox &source) :
		IBounding(),
		m_minExtents(source.m_minExtents),
		m_maxExtents(source.m_maxExtents)
	{
	}

	BoundingBox::~BoundingBox()
	{
	}

	BoundingBox BoundingBox::Scale(const Vector3 &scale)
	{
		Vector3 minExtents = Vector3(m_minExtents.m_x * scale.m_x,
			m_minExtents.m_y * scale.m_y, m_minExtents.m_z * scale.m_z);
		Vector3 maxExtents = Vector3(m_maxExtents.m_x * scale.m_x,
			m_maxExtents.m_y * scale.m_y, m_maxExtents.m_z * scale.m_z);

		return BoundingBox(minExtents, maxExtents);
	}

	BoundingBox BoundingBox::Expand(const Vector3 &expand)
	{
		Vector3 minExtents = Vector3(m_minExtents.m_x - expand.m_x,
			m_minExtents.m_y - expand.m_y, m_minExtents.m_z - expand.m_z);
		Vector3 maxExtents = Vector3(m_maxExtents.m_x + expand.m_x,
			m_maxExtents.m_y + expand.m_y, m_maxExtents.m_z + expand.m_z);

		return BoundingBox(minExtents, maxExtents);
	}

	BoundingBox BoundingBox::Combine(const BoundingBox &other)
	{
		float newMinX = std::min(m_minExtents.m_x, other.m_minExtents.m_x);
		float newMinY = std::min(m_minExtents.m_y, other.m_minExtents.m_y);
		float newMinZ = std::min(m_minExtents.m_z, other.m_minExtents.m_z);
		float newMaxX = std::max(m_maxExtents.m_x, other.m_maxExtents.m_x);
		float newMaxY = std::max(m_maxExtents.m_y, other.m_maxExtents.m_y);
		float newMaxZ = std::max(m_maxExtents.m_z, other.m_maxExtents.m_z);

		Vector3 minExtents = Vector3(newMinX, newMinY, newMinZ);
		Vector3 maxExtents = Vector3(newMaxX, newMaxY, newMaxZ);

		return BoundingBox(minExtents, maxExtents);
	}

	BoundingBox BoundingBox::Stretch(const Vector3 &stretch)
	{
		float newMinX, newMaxX, newMinY, newMaxY, newMinZ, newMaxZ;

		if (stretch.m_x < 0.0f)
		{
			newMinX = m_minExtents.m_x + stretch.m_x;
			newMaxX = m_maxExtents.m_x;
		}
		else
		{
			newMinX = m_minExtents.m_x;
			newMaxX = m_maxExtents.m_x + stretch.m_x;
		}

		if (stretch.m_y < 0.0f)
		{
			newMinY = m_minExtents.m_y + stretch.m_y;
			newMaxY = m_maxExtents.m_y;
		}
		else
		{
			newMinY = m_minExtents.m_y;
			newMaxY = m_maxExtents.m_y + stretch.m_y;
		}

		if (stretch.m_z < 0.0f)
		{
			newMinZ = m_minExtents.m_z + stretch.m_z;
			newMaxZ = m_maxExtents.m_z;
		}
		else
		{
			newMinZ = m_minExtents.m_z;
			newMaxZ = m_maxExtents.m_z + stretch.m_z;
		}

		Vector3 minExtents = Vector3(newMinX, newMinY, newMinZ);
		Vector3 maxExtents = Vector3(newMaxX, newMaxY, newMaxZ);

		return BoundingBox(minExtents, maxExtents);
	}

	void BoundingBox::Load(LoadedValue *value)
	{
	}

	void BoundingBox::Write(LoadedValue *destination)
	{
	}

	IBounding *BoundingBox::UpdateCollider(const Transform &transform, IBounding *destination)
	{
		auto source = dynamic_cast<BoundingBox *>(destination);

		if (source == nullptr)
		{
			delete destination;
			source = new BoundingBox();
			destination = source;
		}

		// Sets the destinations values to the sources.
		source->m_minExtents = m_minExtents;
		source->m_maxExtents = m_maxExtents;

		// Scales the dimensions for the aabb.
		if (transform.GetScaling() != 1.0f)
		{
			source->m_minExtents = Vector3(source->m_minExtents.m_x * transform.GetScaling().m_x, source->m_minExtents.m_y * transform.GetScaling().m_y, source->m_minExtents.m_z * transform.GetScaling().m_z);
			source->m_maxExtents = Vector3(source->m_maxExtents.m_x * transform.GetScaling().m_x, source->m_maxExtents.m_y * transform.GetScaling().m_y, source->m_maxExtents.m_z * transform.GetScaling().m_z);
		}

		// Creates the 8 aabb corners and rotates them.
		if (transform.GetRotation() != 0.0f)
		{
			// TODO: Check out rotations.

			Vector3 fll = Vector3(source->m_minExtents.m_x, source->m_minExtents.m_y, source->m_minExtents.m_z);
		//	fll = fll.Rotate(transform.GetRotation());

			Vector3 flr = Vector3(source->m_maxExtents.m_x, source->m_minExtents.m_y, source->m_minExtents.m_z);
		//	flr = flr.Rotate(transform.GetRotation());

			Vector3 ful = Vector3(source->m_minExtents.m_x, source->m_maxExtents.m_y, source->m_minExtents.m_z);
		//	ful = ful.Rotate(transform.GetRotation());

			Vector3 fur = Vector3(source->m_maxExtents.m_x, source->m_maxExtents.m_y, source->m_minExtents.m_z);
		//	fur = fur.Rotate(transform.GetRotation());

			Vector3 bur = Vector3(source->m_maxExtents.m_x, source->m_maxExtents.m_y, source->m_maxExtents.m_z);
		//	bur = bur.Rotate(transform.GetRotation());

			Vector3 bul = Vector3(source->m_minExtents.m_x, source->m_maxExtents.m_y, source->m_maxExtents.m_z);
		//	bul = bul.Rotate(transform.GetRotation());

			Vector3 blr = Vector3(source->m_maxExtents.m_x, source->m_minExtents.m_y, source->m_maxExtents.m_z);
		//	blr = blr.Rotate(transform.GetRotation());

			Vector3 bll = Vector3(source->m_minExtents.m_x, source->m_minExtents.m_y, source->m_maxExtents.m_z);
		//	bll = bll.Rotate(transform.GetRotation());

			//source->m_minExtents = min(fll, min(flr, min(ful, min(fur, min(bur, min(bul, min(blr, bll)))))));
			source->m_minExtents = Vector3::MinVector(fll, flr);
			source->m_minExtents = Vector3::MinVector(source->m_minExtents, ful);
			source->m_minExtents = Vector3::MinVector(source->m_minExtents, fur);
			source->m_minExtents = Vector3::MinVector(source->m_minExtents, bur);
			source->m_minExtents = Vector3::MinVector(source->m_minExtents, bul);
			source->m_minExtents = Vector3::MinVector(source->m_minExtents, blr);
			source->m_minExtents = Vector3::MinVector(source->m_minExtents, bll);

			//source->m_maxExtents = max(fll, max(flr, max(ful, max(fur, max(bur, max(bul, max(blr, bll)))))));
			source->m_maxExtents = Vector3::MaxVector(fll, flr);
			source->m_maxExtents = Vector3::MaxVector(source->m_maxExtents, ful);
			source->m_maxExtents = Vector3::MaxVector(source->m_maxExtents, fur);
			source->m_maxExtents = Vector3::MaxVector(source->m_maxExtents, bur);
			source->m_maxExtents = Vector3::MaxVector(source->m_maxExtents, bul);
			source->m_maxExtents = Vector3::MaxVector(source->m_maxExtents, blr);
			source->m_maxExtents = Vector3::MaxVector(source->m_maxExtents, bll);
		}

		// Transforms the aabb.
		if (transform.GetPosition() != 0.0f)
		{
			source->m_minExtents = source->m_minExtents + transform.GetPosition();
			source->m_maxExtents = source->m_maxExtents + transform.GetPosition();
		}

		// Returns the final aabb.
		return destination;
	}

	Intersect BoundingBox::Intersects(const IBounding &other)
	{
		auto aabb2 = dynamic_cast<const BoundingBox &>(other);

		Vector3 distance1 = m_minExtents - aabb2.m_maxExtents;
		Vector3 distance2 = aabb2.m_minExtents - m_maxExtents;
		Vector3 maxDistance = Vector3::MaxVector(distance1, distance2);
		float maxDist = maxDistance.MaxComponent();

		return Intersect(maxDist < 0.0f, maxDist);

		/*else if (dynamic_cast<BoundingSphere*>(other) != 0)
		{
			BoundingSphere *sphere = dynamic_cast<BoundingSphere*>(other);

			float distanceSquared = sphere->getRadius() * sphere->getRadius();

			if (sphere->getPosition()->m_x < m_minExtents.m_x)
			{
				distanceSquared -= pow(sphere->getPosition()->m_x - m_minExtents.m_x, 2);
			}
			else if (sphere->getPosition()->m_x > m_maxExtents.m_x)
			{
				distanceSquared -= pow(sphere->getPosition()->m_x - m_maxExtents.m_x, 2);
			}

			if (sphere->getPosition()->m_y < m_minExtents.m_x)
			{
				distanceSquared -= pow(sphere->getPosition()->m_y - m_minExtents.m_y, 2);
			}
			else if (sphere->getPosition()->m_x > m_maxExtents.m_x)
			{
				distanceSquared -= pow(sphere->getPosition()->m_y - m_maxExtents.m_y, 2);
			}

			if (sphere->getPosition()->m_z < m_minExtents.m_x)
			{
				distanceSquared -= pow(sphere->getPosition()->m_z - m_minExtents.m_z, 2);
			}
			else if (sphere->getPosition()->m_z > m_maxExtents.m_x)
			{
				distanceSquared -= pow(sphere->getPosition()->m_z - m_maxExtents.m_z, 2);
			}

			return new intersect(distanceSquared > 0.0f, sqrt(distanceSquared));
		}*/
	}

	Intersect BoundingBox::Intersects(const Ray &ray)
	{
		float tmin = (m_minExtents.m_x - ray.GetOrigin().m_x) / ray.GetCurrentRay().m_x;
		float tmax = (m_maxExtents.m_x - ray.GetOrigin().m_x) / ray.GetCurrentRay().m_x;

		if (tmin > tmax)
		{
			float temp = tmax;
			tmax = tmin;
			tmin = temp;
		}

		float tymin = (m_minExtents.m_y - ray.GetOrigin().m_y) / ray.GetCurrentRay().m_y;
		float tymax = (m_maxExtents.m_y - ray.GetOrigin().m_y) / ray.GetCurrentRay().m_y;

		if (tymin > tymax)
		{
			float temp = tymax;
			tymax = tymin;
			tymin = temp;
		}

		if ((tmin > tymax) || (tymin > tmax))
		{
			return Intersect(false, 0.0f);
		}

		if (tymin > tmin)
		{
			tmin = tymin;
		}

		if (tymax < tmax)
		{
			tmax = tymax;
		}

		float tzmin = (m_minExtents.m_z - ray.GetOrigin().m_z) / ray.GetCurrentRay().m_z;
		float tzmax = (m_maxExtents.m_z - ray.GetOrigin().m_z) / ray.GetCurrentRay().m_z;

		if (tzmin > tzmax)
		{
			float temp = tzmax;
			tzmax = tzmin;
			tzmin = temp;
		}

		return Intersect(!(tmin > tzmax || tzmin > tmax), 0.0f);
	}

	bool BoundingBox::InFrustum(const Frustum &frustum)
	{
		return frustum.CubeInFrustum(m_minExtents, m_maxExtents);
	}

	bool BoundingBox::Contains(const IBounding &other)
	{
		auto aabb2 = dynamic_cast<const BoundingBox &>(other);

		return m_minExtents.m_x <= aabb2.m_minExtents.m_x &&
			aabb2.m_maxExtents.m_x <= m_maxExtents.m_x &&
			m_minExtents.m_y <= aabb2.m_minExtents.m_y &&
			aabb2.m_maxExtents.m_y <= m_maxExtents.m_y &&
			m_minExtents.m_z <= aabb2.m_minExtents.m_z &&
			aabb2.m_maxExtents.m_z <= m_maxExtents.m_z;
	}

	bool BoundingBox::Contains(const Vector3 &point)
	{
		if (point.m_x > m_maxExtents.m_x)
		{
			return false;
		}

		if (point.m_x < m_minExtents.m_x)
		{
			return false;
		}

		if (point.m_y > m_maxExtents.m_y)
		{
			return false;
		}

		if (point.m_y < m_minExtents.m_y)
		{
			return false;
		}

		if (point.m_z > m_maxExtents.m_z)
		{
			return false;
		}

		if (point.m_z < m_minExtents.m_z)
		{
			return false;
		}

		return true;
	}

	float BoundingBox::GetCentreX() const
	{
		return (m_minExtents.m_x + m_maxExtents.m_x) / 2.0f;
	}

	float BoundingBox::GetCentreY() const
	{
		return (m_minExtents.m_y + m_maxExtents.m_y) / 2.0f;
	}

	float BoundingBox::GetCentreZ() const
	{
		return (m_minExtents.m_z + m_maxExtents.m_z) / 2.0f;
	}

	float BoundingBox::GetWidth() const
	{
		return m_maxExtents.m_x - m_minExtents.m_x;
	}

	float BoundingBox::GetHeight() const
	{
		return m_maxExtents.m_y - m_minExtents.m_y;
	}

	float BoundingBox::GetDepth() const
	{
		return m_maxExtents.m_z - m_minExtents.m_z;
	}

	BoundingBox &BoundingBox::operator=(const BoundingBox &other)
	{
		m_minExtents = other.m_minExtents;
		m_maxExtents = other.m_maxExtents;
		return *this;
	}

	bool BoundingBox::operator==(const BoundingBox &other) const
	{
		return m_minExtents == other.m_minExtents && m_maxExtents == other.m_maxExtents;
	}

	bool BoundingBox::operator!=(const BoundingBox &other) const
	{
		return !(*this == other);
	}
}