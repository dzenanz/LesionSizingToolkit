/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkLesionSegmentationMethod.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLesionSegmentationMethod_h
#define __itkLesionSegmentationMethod_h

#include "itkProcessObject.h"
#include "itkImage.h"
#include "itkDataObjectDecorator.h"
#include "itkSpatialObject.h"
#include "itkFeatureGenerator.h"
#include "itkSegmentationModule.h"

namespace itk
{

/** \class LesionSegmentationMethod
 * \brief Class for coordinating components of a segmentation framework.
 *
 * This class was designed to facilitate the segmentation of lung lesions, but
 * yet to be useful for other segmentation tasks.
 *
 * SpatialObjects are used as inputs and outputs of this class.
 *
 * \ingroup SpatialObjectFilters
 */
template <unsigned int NDimension>
class ITK_EXPORT LesionSegmentationMethod : public ProcessObject
{
public:
  /** Standard class typedefs. */
  typedef LesionSegmentationMethod      Self;
  typedef ProcessObject                 Superclass;
  typedef SmartPointer<Self>            Pointer;
  typedef SmartPointer<const Self>      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(LesionSegmentationMethod, ProcessObject);

  /** Dimension of the space */
  itkStaticConstMacro(Dimension, unsigned int, NDimension);

  /** Type of spatialObject that will be passed as input and output of this
   * segmentation method. */
  typedef SpatialObject< NDimension >           SpatialObjectType;
  typedef typename SpatialObjectType::Pointer   SpatialObjectPointer;

  /** SpatialObject that defines the Region of Interest in the input data */
  itkSetObjectMacro( RegionOfInterest, SpatialObjectType );
  itkGetConstObjectMacro( RegionOfInterest, SpatialObjectType );

  /** SpatialObject that defines the initial segmentation. This will be
   * used to initialize the segmentation process driven by the
   * LesionSegmentationMethod. */
  itkSetObjectMacro( InitialSegmentation, SpatialObjectType );
  itkGetConstObjectMacro( InitialSegmentation, SpatialObjectType );

  /** Type of the class that will generate input features in the form of
   * spatial objects. */
  typedef FeatureGenerator< Dimension >         FeatureGeneratorType;

  /**
   * Method for adding a feature generator that will compute the Nth feature to
   * be passed to the segmentation module.
   */
  void AddFeatureGenerator( FeatureGeneratorType * generator ); 

  /** Type of the segmentation module that encapsulate the actual segmentation
   * algorithm. */
  typedef SegmentationModule< Dimension >             SegmentationModuleType;
  typedef typename SegmentationModuleType::Pointer    SegmentationModulePointer;


  /**
   * Method for setting the class that encapsulates the actual segmentation
   * algorithm.
   */
  void SetSegmentationModule( SegmentationModuleType * segmentor ); 


protected:
  LesionSegmentationMethod();
  virtual ~LesionSegmentationMethod();
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** Method invoked by the pipeline in order to trigger the computation of
   * the segmentation. */
  void  GenerateData ();

private:
  LesionSegmentationMethod(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  SpatialObjectPointer                      m_RegionOfInterest;
  SpatialObjectPointer                      m_InitialSegmentation;
  
  typedef typename FeatureGeneratorType::Pointer                FeatureGeneratorPointer;
  typedef std::vector< FeatureGeneratorPointer >                FeatureGeneratorArrayType;
  typedef typename FeatureGeneratorArrayType::iterator          FeatureGeneratorIterator;
  typedef typename FeatureGeneratorArrayType::const_iterator    FeatureGeneratorConstIterator;

  FeatureGeneratorArrayType                 m_FeatureGenerators;

  SegmentationModulePointer                 m_SegmentationModule;


  void UpdateAllFeatureGenerators();

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
# include "itkLesionSegmentationMethod.txx"
#endif

#endif
