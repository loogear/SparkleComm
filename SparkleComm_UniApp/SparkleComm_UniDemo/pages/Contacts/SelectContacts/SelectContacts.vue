<template>
	<form class='loginView' @submit="CallTrans">
		<view class="input-view">
			<view class="label-view">
				<text class="label">号码</text>
			</view>
			<input class="input" type="text" placeholder="请输入号码" name="nameValue" />
		</view>
		<view class="button-view">
			<button type="default" class="login" hover-class="hover" formType="submit">转移</button>
		</view>
	</form>
</template>
<script>
	const modal = uni.requireNativePlugin('modal');
	const PhoneSdk = uni.requireNativePlugin('SparkleComm_Plugin');
	
	export default {
		data() {
			return {
				title: '选择联系人',
				data:"",
				CallID:-1,
				AccountID:-1,
				ShowInCall:true // 退出本页面后，是否显示通话界面
			}
		},
		onLoad(e){
			if(e.data){
				this.data = e.data;
			}
			this.CallID = e.CallID;
			this.AccountID = e.AccountID;
		},
		onBackPress(options) {  
			console.log("onBackPress:"+options.from);
			if(this.ShowInCall) {// 取消时，返回后重新显示通话界面
				console.log("onBackPress:ShowInCall");
				PhoneSdk.ShowInCall({
				}, result => {
					const msg = JSON.stringify(result);
					console.log("CallTransfer:result=" + msg);
				});
			}
			return false;
		},  
		methods: {
			CallTrans(e) {
				console.log("CallTrans:" + e.detail.value.nameValue + " CallID="+this.CallID+" AccountID=" + this.AccountID);
				
				PhoneSdk.CallTransfer({
					addr: e.detail.value.nameValue,
					callID: this.CallID,
					accountID: this.AccountID
				}, result => {
					const msg = JSON.stringify(result);
					console.log("CallTransfer:result=" + msg);
				});
				this.ShowInCall = false;
				uni.navigateBack();
			},
		}
	}
</script>
<style>
	page{
		display: flex;
		min-height: 100%;
	}
	.root{
		display: flex;
		flex: 1;
		flex-direction: column;
	}
	.page-body{
		flex: 1;
		display: flex;
		justify-content: center;
		align-items: center;
	}
</style>