/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkCannyEdgesFeatureGenerator.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkCannyEdgesFeatureGenerator_txx
#define __itkCannyEdgesFeatureGenerator_txx

#include "itkCannyEdgesFeatureGenerator.h"


namespace itk
{

/**
 * Constructor
 */
template <unsigned int NDimension>
CannyEdgesFeatureGenerator<NDimension>
::CannyEdgesFeatureGenerator()
{
  this->SetNumberOfRequiredInputs( 1 );

  this->m_CastFilter        = CastFilterType::New();
  this->m_RescaleFilter     = RescaleFilterType::New();
  this->m_CannyFilter       = CannyEdgeFilterType::New();

  typename OutputImageSpatialObjectType::Pointer 
    outputObject = OutputImageSpatialObjectType::New();

  this->ProcessObject::SetNthOutput( 0, outputObject.GetPointer() );

  this->m_Sigma =  1.0;
  this->m_UpperThreshold = NumericTraits< InternalPixelType >::max();
  this->m_LowerThreshold = NumericTraits< InternalPixelType >::min();

  this->m_RescaleFilter->SetOutputMinimum( 0.0 );
  this->m_RescaleFilter->SetOutputMaximum( 1.0 );
}


/*
 * Destructor
 */
template <unsigned int NDimension>
CannyEdgesFeatureGenerator<NDimension>
::~CannyEdgesFeatureGenerator()
{
}

template <unsigned int NDimension>
void
CannyEdgesFeatureGenerator<NDimension>
::SetInput( const SpatialObjectType * spatialObject )
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(0, const_cast<SpatialObjectType *>( spatialObject ));
}

template <unsigned int NDimension>
const typename CannyEdgesFeatureGenerator<NDimension>::SpatialObjectType *
CannyEdgesFeatureGenerator<NDimension>
::GetFeature() const
{
  return static_cast<const SpatialObjectType*>(this->ProcessObject::GetOutput(0));
}


/*
 * PrintSelf
 */
template <unsigned int NDimension>
void
CannyEdgesFeatureGenerator<NDimension>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf( os, indent );
}


/*
 * Generate Data
 */
template <unsigned int NDimension>
void
CannyEdgesFeatureGenerator<NDimension>
::GenerateData()
{
  typename InputImageSpatialObjectType::ConstPointer inputObject =
    dynamic_cast<const InputImageSpatialObjectType * >( this->ProcessObject::GetInput(0) );

  if( !inputObject )
    {
    itkExceptionMacro("Missing input spatial object or incorrect type");
    }

  const InputImageType * inputImage = inputObject->GetImage();

  if( !inputImage )
    {
    itkExceptionMacro("Missing input image");
    }

  this->m_CastFilter->SetInput( inputImage );
  this->m_CannyFilter->SetInput( this->m_CastFilter->GetOutput() );
  this->m_RescaleFilter->SetInput( this->m_CannyFilter->GetOutput() );

  this->m_CannyFilter->SetVariance( this->m_Sigma * this->m_Sigma );
  this->m_CannyFilter->SetUpperThreshold( this->m_UpperThreshold );
  this->m_CannyFilter->SetLowerThreshold( this->m_LowerThreshold );
  this->m_CannyFilter->SetMaximumError(0.01);
  this->m_CannyFilter->SetOutsideValue(NumericTraits<InternalPixelType>::Zero);

  this->m_RescaleFilter->Update();

  typename OutputImageType::Pointer outputImage = this->m_RescaleFilter->GetOutput();

  outputImage->DisconnectPipeline();

  typedef ImageSpatialObject< Dimension, OutputPixelType > OutputImageSpatialObjectType;

  OutputImageSpatialObjectType * outputObject =
    dynamic_cast< OutputImageSpatialObjectType * >(this->ProcessObject::GetOutput(0));

  outputObject->SetImage( outputImage );
}

} // end namespace itk

#endif