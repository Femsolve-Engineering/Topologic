// This file is part of Topologic software library.
// Copyright(C) 2019, Cardiff University and University College London
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#include <Aperture.h>
#include <ApertureFactory.h>
#include <Context.h>
#include <Face.h>
#include <Vertex.h>

#include <assert.h>
#include <array>

namespace TopologicCore
{
	PROCESSED Aperture::Aperture(
		const Topology::Ptr& kpTopology,
		const std::shared_ptr<Context>& kpContext, 
		const std::string& rkGuid)
		: TopologicCore::Topology(kpTopology->Dimensionality(), kpTopology->GetOcctShape(), rkGuid.compare("") == 0 ? GetClassGUID() : rkGuid)
		, m_pMainContext(kpContext)
		, m_pTopology(kpTopology)
	{
		RegisterFactory(GetClassGUID(), std::make_shared<ApertureFactory>());
		if (kpTopology == nullptr)
		{
			throw std::runtime_error("A null topology is passed.");
		}

		if (kpContext != nullptr)
		{
			AddContext(kpContext);
		}
	}

	IGNORED_BECAUSE_NOT_NEEDED Aperture::Ptr Aperture::ByTopologyContext(const Topology::Ptr& kpTopology, const Context::Ptr& kpContext)
	{
		return std::make_shared<Aperture>(kpTopology, kpContext);
	}

	PROCESSED Aperture::Ptr Aperture::ByTopologyContext(
		const Topology::Ptr& kpTopology, 
		const Topology::Ptr& kpContextTopology)
	{
		const double kDefaultParameter = 0.0;

		// Identify the closest simplest subshape
		Topology::Ptr pClosestSimplestSubshape 
			= kpContextTopology->ClosestSimplestSubshape(kpTopology->CenterOfMass());

		// Create a Context from kpContextTopology
		Context::Ptr pContext = Context::ByTopologyParameters(
			pClosestSimplestSubshape, 
			kDefaultParameter, 
			kDefaultParameter, 
			kDefaultParameter);

		// Create the Aperture
		Aperture::Ptr pAperture = ByTopologyContext(kpTopology, pContext);
		
		return pAperture;
	}

	PROCESSED Vertex::Ptr Aperture::CenterOfMass() const
	{
		return Topology()->CenterOfMass();
	}

	PROCESSED bool Aperture::IsManifold(const Topology::Ptr& kpHostTopology) const
	{
		return Topology()->IsManifold(kpHostTopology);
	}

	PROCESSED std::string Aperture::GetTypeAsString() const
	{
		return std::string("Aperture");
	}

	PROCESSED void Aperture::Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const
	{
		Topology()->Geometry(rOcctGeometries);
	}

	PROCESSED void Aperture::SetOcctShape(const TopoDS_Shape & rkOcctShape)
	{
		m_pTopology->SetOcctShape(rkOcctShape);
	}

	PROCESSED TopoDS_Shape& Aperture::GetOcctShape()
	{
		return Topology()->GetOcctShape();
	}

	PROCESSED const TopoDS_Shape& Aperture::GetOcctShape() const
	{
		return Topology()->GetOcctShape();
	}

	PROCESSED TopologyType Aperture::GetType() const
	{
		return TOPOLOGY_APERTURE;
	}

	PROCESSED bool Aperture::IsContainerType()
	{
		return Topology()->IsContainerType();
	}

	PROCESSED TopoDS_Shape Aperture::OcctShapeFix(const TopoDS_Shape & rkOcctInputShape)
	{
		// No shape fix method attached to Aperture
		return rkOcctInputShape;
	}

	PROCESSED Topology::Ptr Aperture::Topology() const
	{
		assert(m_pTopology != nullptr && "The underlying topology is null.");
		if (m_pTopology == nullptr)
		{
			throw std::runtime_error("The underlying topology is null.");
		}
		return m_pTopology;
	}

	PROCESSED Aperture::~Aperture()
	{
		// Does not delete the contents; if the aperture disappears, its contents continue to exist.
	}
}
