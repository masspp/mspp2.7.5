/**
 * @file PluginManagerPluginManager.h
 * @brief interfaces of PluginManager class
 *
 * @author okada_h
 * @date 2013.02.19
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */



#ifndef __KOME_PLUGIN_MANAGER_PLUGIN_MANAGER_H__
#define __KOME_PLUGIN_MANAGER_PLUGIN_MANAGER_H__


#define STR_PACKAGE_FULL		"Full"
#define STR_PACKAGE_CUSTOMIZED	"Customized"

// ON/OFF�ꎞ�ۊǃe�[�u���p
typedef struct tag_active_table {	
	std::string strPluginName;		// Plugin Name
	int nPluginIndex;				// Plugin Index(PluginManager::getPlugin(index)����index�ԍ�)
	
//	std::string strFunctionName;	// Function Name
//	unsigned int uFunctionIndex;	// Function Index(PluginInfo->getCall(index)����index�ԍ�)

	std::string strComponentName;	// Component Name
	unsigned int uComponentIndex;	// Component Index(PluginInfo->getCall(index)����index�ԍ�)


	bool flgActiveTemp;				// Active
} ACTIVE_TABLE, *LPACTIVE_TABLE ;

namespace kome{
	namespace pluginmanager{
		namespace view{

			// class�̑O���Q�Ɨp�L�ڗ�
			class PluginManagerDlg;

			/**
			 * @class PluginManagerView
			 * @brief PluginManager view manager
			 */
			class PluginManagerView {
			protected:
				/**
				 * @fn PluginManagerView()
				 * @brief constructor
				 */
				PluginManagerView();

				/**
				 * @fn virtual ~PluginManagerView()
				 * @brief destructor
				 */
				virtual ~PluginManagerView();

			private:
				/** dlg */
				PluginManagerDlg* m_dlg;

				/** active plugin-function table */
				std::vector< ACTIVE_TABLE >	m_vectActiveTable;


			public:
				
				/**
				 * @fn int openDialog();
				 * @brief open dialog
				 * @return wxID_APPLY:[Apply]�{�^���������ďI�� 
				 */
				int openDialog();
				
			public:
				/**
				 * @fn int onSelectPackage(); 
				 * @brief on select package
				 * @return -1:fail 0:success
				 */
				int onSelectPackage();

			public:
				/**
				 * @fn PluginManagerDlg* getDlg(); 
				 * @brief get dialog
				 * @return dialog
				 */
				PluginManagerDlg* getDlg();

				/**
				 * @fn void createActiveTable(); 
				 * @brief create active table
				 */
				void createActiveTable();

				/**
				 * @fn void clearActiveTable(); 
				 * @brief clear active table
				 */
				void clearActiveTable();

				/**
				 * @fn int checkActiveTableCompo( int nPluginIndex, unsigned int uFunctionIndex ); 
				 * @brief is ative table
				 * @return 1:true 0:false -1:nothing -2:Parameter error
				 */
				int checkActiveTableCompo( int nPluginIndex, unsigned int uComponentIndex );

				/**
				 * @fn bool isActiveTable( int nPluginIndex, unsigned int uFunctionIndex ); 
				 * @brief is ative table
				 * @return 1:true 0:false -1:nothing -2:Parameter error
				 */
				
				bool isActiveTableCompo( int nPluginIndex, unsigned int uComponentIndex );
				/**
				 * @fn int updateActiveTableCompo( int nPluginIndex, unsigned int uFunctionIndex, bool flgActive ); 
				 * @brief update active table
				 * @return 1:true(�l�ݒ萬��) 0:false(�l�ݒ萬��) -1:nothing -2:Parameter error
				 */
				int updateActiveTableCompo( int nPluginIndex, unsigned int uComponentIndex, bool flgActive );

				/**
				 * @fn void writeMemoryActiveTable(); 
				 * @brief write memory active table
				 */
				void writeMemoryActiveTable();

				/**
				 * @fn void saveActiveTable(); 
				 * @brief ActiveTable��INI�t�@�C���ɕۑ�
				 */
				void saveActiveTable();

				/**
				 * @fn int selectPackage( int index ); 
				 * @brief select package
				 * @param[in] index index of package
				 * @return 0
				 */
				int selectPackage( int index );

				/**
				 * @fn int selectPackage( const char* pszPackage ); 
				 * @brief select package
				 * @param[in] pszPackage name of package
				 * @return 0
				 */
				int selectPackage( const char* pszPackage );


				/**
				 * @fn static PluginManagerView& getInstance()
				 * @brief gets PluginManagerView object (This is the only object.)
				 * @return PluginManagerView view manager object
				 */
				static PluginManagerView& getInstance();
				
			};
		}
	}
}





#endif    // __KOME_PLUGIN_MANAGER_PLUGIN_MANAGER_H__
