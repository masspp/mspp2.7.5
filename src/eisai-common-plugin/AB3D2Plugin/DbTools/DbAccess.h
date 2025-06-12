/*********************************************************/
/*!
 @addtoGroup AB3D
 @file DBAccess.h
 @brief AB3D Peak Position DB Accesser Base
 
 @date 2013.07.01
 @author M.Fukuda
 @varsion 1.0.0

 -------------------------------------------------------
 Copyright(C) 2006-2014 
 Eisai Co., Ltd. All rights reserved.
 -------------------------------------------------------
 @note
 */
/*********************************************************/
#pragma once

namespace AB3D {

/*!--------------------------------------------------------
    @name   DbAccess
    @brief  Data Base Access Class
    @date   2013.07.01
    @par    IdentificationManager �̎����Ɉˑ������A
            DB�𑀍삷��ׂ�SQL,STMT�̒�`���Ǘ�����B
-----------------------------------------------------------*/
	class DbAccess {

	public:
		// constructor
		DbAccess();
		// destructor
		~DbAccess();

	private:
		// Uncopyable
		DbAccess(const DbAccess&);
		DbAccess& operator=(const DbAccess&);

	public:
//		const bool setup();

		// Only Action
		const int getMaxID(const char* tablename);

		/*!
		 * @brief TempTable���쐬����
		 * @param[in]  %TABLE% ��
		 * @param[in]  �e�[�u���������`�����֐���n��
		 * @retval     �����Ȃ�T���Ԃ�
		 * @note
		      �e�[�u���̐����֐��� addcolumn�Ŋe�X��`�������̂�n���B
		 */
		const bool createTempTable(
			const char* tablename, 
			void (*ctFnc)(kome::db::Table& table, const char* tname));

		//------------------------------------------------
		// for create Statemet
		//------------------------------------------------

		/*!
		 * @brief SQL��Stmt�ɃZ�b�g����
		 * @param[in]  SQL��
		 * @param[out] ����SQL���Z�b�g����statement.
		 * @retval     �����Ȃ�True���Ԃ�
		 */
		const bool createStmt(
			const std::string& sql,
			kome::db::Statement*& refstmt);

		/*!
		 * @brief Select��SQL��Stmt�ɃZ�b�g����
		 * @param[in]  %TABLE% ��
		 * @param[in]  select����v�f
		 * @param[in]  where�̗v�f
		 * @param[out] ����SQL���Z�b�g����statement(�����Ń��Z�b�g�����)
		 * @retval     �����Ȃ�O���Ԃ�
		 */
		const int createSelectStmt(
			const char* tname,
			const std::vector<const std::string>& sctVals, 
			const std::vector<const std::string>& whereVals,
			std::unique_ptr<kome::db::Statement>& refstmt);

		/*!
		 * @brief Insert��SQL��Stmt�ɃZ�b�g����
		 * @param[in]  %TABLE% ��
		 * @param[in]  insert����v�f
		 * @param[out] ����SQL���Z�b�g����statement.
		 * @retval     �����Ȃ�O���Ԃ�
		 */
		const int createInsertStmt(
			const char* tname,
			const std::vector<const std::string>& vals,
			std::unique_ptr<kome::db::Statement>& refstmt);

		/*!
		 * @brief update��SQL��Stmt�ɃZ�b�g����
		 * @param[in]  %TABLE% ��
		 * @param[in]  update����v�f
		 * @param[in]  where�̗v�f
		 * @param[out] ����SQL���Z�b�g����statement.
		 * @retval     �����Ȃ�O���Ԃ�
		 */
		const int createUpdateStmt(
			const char* tablename, 
			const std::vector<const std::string>& setVals, 
			const std::vector<const std::string>& whereVals,
			std::unique_ptr<kome::db::Statement>& refstmt);

	private:
		/*!
		 * @brief  DB�̃|�C���^���擾����
		 * @retval DB�̃|�C���^
		 */
		kome::db::Database* onGetDB();

		//-------------------------------------------------
		// on Prepare Statment
		//-------------------------------------------------

		/*!
		 * @brief Select��SQL��Stmt�ɃZ�b�g����
		 * @param[in]  %TABLE% ��
		 * @param[in]  select����v�f
		 * @param[in]  where�̗v�f
		 * @param[out] ����SQL���Z�b�g����statement.
		 * @retval     �����Ȃ�O���Ԃ�
		 */
		const int setStatement_Select(
			const char* tablename, 
			const std::vector<const std::string>& setVals, 
			const std::vector<const std::string>& whereVals,
			kome::db::Statement*& refstmt);

		/*!
		 * @brief Insert��SQL��Stmt�ɃZ�b�g����
		 * @param[in]  %TABLE% ��
		 * @param[in]  insert����v�f
		 * @param[out] ����SQL���Z�b�g����statement.
		 * @retval     �����Ȃ�O���Ԃ�
		 */
		const int setStatement_Insert(
			const char* tablename, 
			const std::vector<const std::string>& intoVals,
			kome::db::Statement*& refstmt);

		/*!
		 * @brief update��SQL��Stmt�ɃZ�b�g����
		 * @param[in]  %TABLE% ��
		 * @param[in]  update����v�f
		 * @param[in]  where�̗v�f
		 * @param[out] ����SQL���Z�b�g����statement.
		 * @retval     �����Ȃ�O���Ԃ�
		 */
		const int setStatement_Update(
			const char* tablename, 
			const std::vector<const std::string>& setVals, 
			const std::vector<const std::string>& whereVals,
			kome::db::Statement*& refstmt);

		//-------------------------------------------------
		// �z�񂩂�SQL�ŗL�̌J��Ԃ����������
		//-------------------------------------------------
		/*!
		 * @brief [select (v1,v2,..) from %TABLE%]�����
		 * @param[in]  %TABLE% ��
		 * @param[in]  select����v�f
		 * @retval     ��������SQL��
		 * @note       where��͕ʓr����
		 */
		const std::string makeSQL_Select(
			const char* talbename,
			const std::vector<const std::string>& vals);

		/*!
		 * @brief [insert into %TABLE% (v1,v2,..) values(?,?,..)]��SQL�����
		 * @param[in]  %TABLE% ��
		 * @param[in]  insert����v�f
		 * @retval     ��������SQL��
		 * @note       where��͕ʓr����
		 */
		const std::string makeSQL_Insert(
			const char* tablename, 
			const std::vector<const std::string>& vals);

		/*!
		 * @brief SQL��[update %TABLE% set val1=?, ...]�����
		 * @param[in]  %TABLE% ��
		 * @param[in]  update����v�f
		 * @retval     ��������SQL��
		 * @note       where��͕ʓr����
		 */
		const std::string makeSQL_Update(
			const char* tablename, 
			const std::vector<const std::string>& vals);

		/*!
		 * @brief SQL��[where st1=? AND st2=?,...]�����
		 * @param[in]  %TABLE% ��
		 * @param[in]  where��̗v�f
		 * @retval     ��������SQL��
		 */
		const std::string makeSQL_Where(
			const std::vector<const std::string>& vals);
	};

/*!--------------------------------------------------------
    @name   setTableInfo
    @brief  �e�[�u���̍쐬��⏕����
    @date   2013.07.01
    @par    DB�e�[�u���쐬�̌ŗL���[����Z�߂�
-----------------------------------------------------------*/
	class setTableInfo {
	public:

		/*!
		 * @brief constructor
		 * @param[in] �e�[�u����
		 * @param[in] �e�[�u���I�u�W�F�N�g
		 */
		setTableInfo(
			const char* tname,
			kome::db::Table& table);

		// destrcutor
		~setTableInfo(){}
	private:
		/* inner elements */
		kome::db::Table& table_;

	public:
		/*!
		 * @brief �e�[�u���v�f�̍쐬
		 * @param[in] �J������
		 * @param[in] �f�[�^�^�C�v(integet,String��)
		 * @param[in] �����͂������Ȃ��Ȃ�True
		 * @param[in] �������Z(ID�̃^�C�v)�Ȃ�True.
		 */
		void addColumn(
			const std::string& colname,
			const kome::db::TableColumn::DataType& coltype,
			bool notNull = false,
			bool autoincrement = false);

	};

}
